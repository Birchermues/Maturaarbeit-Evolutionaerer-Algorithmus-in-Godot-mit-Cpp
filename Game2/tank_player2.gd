extends RigidBody2D

@export var SPEED : float = 100.0
@export var amount_of_raycasts : int = 11

# für die observationen der KI
@export var raycast_fov : int = 180
@export var raycast_dist : float = 1000
@export var raycasts_rotate_with_turret : bool = false
var rays : Array[RayCast2D]

var f_b_speed : float = 0.0
var l_r_speed : float = 0.0

# das neuronale netz des spielers
@onready var neural_net : nn = nn.new()
@export var network_resource : NeuralNetworkRes
@export var max_weight : float = 1.0
@export var max_bias : float = 1.0
@export var use_normal_distribution : bool = true

# belohnung und bestrafung -> einfluss auf den score
@export var kill_reward : float = 1.0
@export var death_penalty : float = 1.5
@export var collision_penalty : float = 0.25

@onready var coll_shape : CollisionShape2D = $CollisionShape2D
@export var reload_time : float = 2.0

func _ready():
	$Line2D.visible = false
	
	var offset : int = 0
	if raycast_fov == 360 and amount_of_raycasts > 1:
		offset = 1
	
	# alle strahlen werden erstellt
	for i in range(amount_of_raycasts):
		var ray : RayCast2D = RayCast2D.new()
		# debug linien zeichnen
		#var line : Line2D = Line2D.new()  
		
		var rot : float = raycast_fov / -2.0 + (i * raycast_fov / (amount_of_raycasts - 1 + offset))
		ray.target_position = Vector2(cos(deg_to_rad(rot)), sin(deg_to_rad(rot))) * raycast_dist
		#line.add_point(Vector2(0, 0), 0)
		#line.add_point(Vector2(cos(deg_to_rad(rot)), sin(deg_to_rad(rot))) * raycast_dist, 0)
		ray.set_collision_mask_value(2, true)
		if raycasts_rotate_with_turret:
			get_node("Turret").add_child(ray)
		else:
			add_child(ray)
	
	if raycasts_rotate_with_turret:
		rays.append_array(get_node("Turret").get_children().filter(func(child : Object): return child.is_class("RayCast2D")))
	else:
		rays.append_array(get_children().filter(func(child : Object): return child.is_class("RayCast2D")))

	# neuronales netz wird erstellt
	neural_net.set_layers(network_resource.layout)
	neural_net.fill_connections()
	
	var weights_and_biases_count : int = 0
	
	for i in range(neural_net.get_layers().size() - 1):
		# layer1 grösse * layer2 grösse steht für alle weights (verbindungen) und die grösse von layer2 für die biases der neuronen
		weights_and_biases_count += neural_net.get_layers()[i] * neural_net.get_layers()[i+1] + neural_net.get_layers()[i+1]
	
	# falls es schon ein netz gibt und es die gleiche anzahl gewichte hat übernehmen wir das
	# so kann man fortschritt speichern und muss nicht jedes mal von vorne trainieren
	if weights_and_biases_count  == network_resource.weights_and_biases.size():
		print("amount of weights and biases provided equals the required amount: ", weights_and_biases_count*2, " are expected and ", network_resource.weights_and_biases.size(), " are provided")
		neural_net.set_weights_and_biases(network_resource.weights_and_biases);
	# falls es noch kein netz vorhanden hat werden die gewichte zufällig generiert
	else:
		print("amount of weights and biases does NOT equal the required amount: ", weights_and_biases_count*2, " are expected and ", network_resource.weights_and_biases.size(), " are provided")
		print("weights and biases will be generated and randomized")
		# die verteilung der gewichte entspricht einer normalverteilung mitstandartabweichung von max_weight und max_bias
		neural_net.randomize_weights_and_biases(use_normal_distribution, max_weight, max_bias)



func _physics_process(delta):
	# input array wird vorbereitet
	var inputs : Array[float] = []
	inputs.resize(rays.size() * 2)
	
	# die raycasts tasten das umfeld ab
	# sie stellen wichtige inputs des neuronalen netzes dar
	var i : int = 0
	for ray in rays:
		# bestimmt die sichtweite in diese richtung
		var coll_dist : float = 0.0
		# bestimmt auf was der strahl getroffen ist
		var coll_type : float = 0.0
		if ray.is_colliding():
			var collision_point : Vector2 = ray.get_collision_point()
			coll_dist = collision_point.distance_to(global_position)
			var obj : Object = ray.get_collider()
			if obj.get_node(".").get_class() == "RigidBody2D":
				# wenn der strahl auf einen spieler getroffen ist
				coll_type = 1.0
			else:
				# wenn der strahl auf eine wand getroffen ist
				coll_type = 0.0
		else:
			# wenn der strahl auf nichts traf
			coll_type = -1.0
		# der erste teil der inputs
		inputs[i] = coll_dist * 0.01 # die distanz wird noch normalisiert
		inputs[i+1] = coll_type
		i += 2

	# der rest der inputs wird hinzugefügt
	inputs.append_array([position.x, position.y, $Turret.rotation])
	
	# vorwärtsiteration des neuronalen netzes
	# hier werden inputs gegeben und outputs ausgerechnet
	var outputs : Array[float] = neural_net.solve(inputs)

	var rot : float = 0.0
	if get_parent().train_ai:
		# für den KI spieler
		# bewegung und rotation je nach outputs des neuronalen netzes
		f_b_speed = clamp(outputs[0], -1.0, 1.0)
		l_r_speed = clamp(outputs[1], -1.0, 1.0)
		rot = fmod(outputs[2], 360.0)
	else:
		# input für den menschlichen spieler
		var vec_to_mouse = get_global_mouse_position() - global_position
		rot = atan2(vec_to_mouse.y, vec_to_mouse.x)
		f_b_speed = Input.get_axis("forward", "backward")
		f_b_speed = Input.get_axis("left", "right")
	
	# der spieler wird bewegt und rotiert
	linear_velocity = Vector2(l_r_speed, f_b_speed).normalized().rotated(rotation - TAU) * SPEED
	get_node("Turret").rotation = rot
	
	# falls der output fürs schiessen über 5 ist, schiesst der spieler
	# 5 wurde so gewählt, dass der spieler nicht zu häufig schiesst
	if outputs[3] > 5.0:
		shoot()
	

# für den menschlichen spieler
func _input(event):
	if event.is_action_pressed("shoot"):
		shoot()
	
# aufgerufen wenn der spieler schaden nimmt, führt zu reset und bestrafung
func damage():
	neural_net.score -= death_penalty
	position = Vector2(randf_range(0, 6200), randf_range(0, 3600))


func shoot():
	if $ReloadTimer.time_left > 0.01:
		return
	
	if $Turret/shoot_raycast.is_colliding():
		var collision_point = $Turret/shoot_raycast.get_collision_point()
		$Line2D.points[1] = collision_point - global_position
		
		var obj : Object = $Turret/shoot_raycast.get_collider()
		
		# falls der spieler einen gegner trifft führt dass zu einer Belohnung und zu schaden am gegner
		if obj.get_node(".").get_class() == "RigidBody2D":
			obj.damage()
			neural_net.score += kill_reward
	else:
		$Line2D.points[1] = Vector2(cos($Turret.rotation), sin($Turret.rotation)) * 3000.0
			
	$ReloadTimer.wait_time = reload_time
	$ReloadTimer.start()
	$Line2D.visible = true
	$Line2D/Timer.start()

func _on_timer_timeout():
	$Line2D.visible = false

# aufgerufen wenn der spieler in einen anderen läuft
func _on_body_entered(body):
	if body.get_class() == "RigidBody2D":
		neural_net.score -= collision_penalty
