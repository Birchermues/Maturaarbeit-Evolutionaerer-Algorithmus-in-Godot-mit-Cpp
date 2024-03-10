extends RigidBody2D

@export var SPEED : float = 100.0
@export var amount_of_raycasts : int = 11
@export var raycast_fov : int = 180
@export var raycast_dist : float = 1000

@export var raycasts_rotate_with_turret : bool = false

var f_b_speed : float = 0.0
var l_r_speed : float = 0.0

@onready var neural_net : nn = nn.new()

@export var network_resource : NeuralNetworkRes

@export var max_weight : float = 1.0
@export var max_bias : float = 1.0
@export var use_normal_distribution : bool = true

@export var kill_reward : float = 1.0
@export var death_penalty : float = 1.5

@onready var coll_shape : CollisionShape2D = $CollisionShape2D
var rays : Array[RayCast2D]

@export var reload_time : float = 2.0
# Called when the node enters the scene tree for the first time.
func _ready():
	$Line2D.visible = false
	
	var offset : int = 0
	if raycast_fov == 360 and amount_of_raycasts > 1:
		offset = 1
	
	for i in range(amount_of_raycasts):
		var ray : RayCast2D = RayCast2D.new()
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

	
	neural_net.set_layers(network_resource.layout)
	neural_net.fill_connections()
	#neural_net.set_weights_and_biases(network_resource.weights_and_biases)
	
	var weights_and_biases_count : int = 0
	
	for i in range(neural_net.get_layers().size() - 1):
		weights_and_biases_count += neural_net.get_layers()[i] * neural_net.get_layers()[i+1]
	
	#weights_and_biases_count *= 2;
	if weights_and_biases_count * 2 == network_resource.weights_and_biases.size():
		print("amount of weights and biases provided equals the required amount: ", weights_and_biases_count*2, " are expected and ", network_resource.weights_and_biases.size(), " are provided")
		neural_net.set_weights_and_biases(network_resource.weights_and_biases);
	else:
		print("amount of weights and biases does NOT equal the required amount: ", weights_and_biases_count*2, " are expected and ", network_resource.weights_and_biases.size(), " are provided")
		print("weights and biases will be generated and randomized")
		neural_net.randomize_weights_and_biases(use_normal_distribution, max_weight, max_bias)



func _physics_process(delta):
	#var rays : Array[RayCast2D]
	#if raycasts_rotate_with_turret:
		#rays = get_node("Turret").get_children().filter()
	var inputs : Array[float] = []
	
	inputs.resize(rays.size() * 2)

	var i : int = 0
	for ray in rays:
		var coll_dist : float = 0.0
		var coll_type : float = 0.0
		if ray.is_colliding():
			var collision_point : Vector2 = ray.get_collision_point()
			coll_dist = collision_point.distance_to(global_position)
			var obj : Object = ray.get_collider()
			if obj.get_node(".").get_class() == "RigidBody2D":
				coll_type = 2.0
			else:
				coll_type = 1.0
		else:
			coll_type = 0.0
		inputs[i] = coll_dist * 0.01
		inputs[i+1] = coll_type
		i += 2


	inputs.append_array([position.x, position.y, $Turret.rotation])
	var outputs : Array[float] = neural_net.solve(inputs)
	#print(outputs)
	var rot : float = 0.0
	if get_parent().train_ai:
		f_b_speed = clamp(outputs[0], -1.0, 1.0)
		l_r_speed = clamp(outputs[1], -1.0, 1.0)
		rot = fmod(outputs[2], 360.0)
		
		#print(f_b_speed)
	else:
		var vec_to_mouse = get_global_mouse_position() - global_position
		rot = atan2(vec_to_mouse.y, vec_to_mouse.x)
		f_b_speed = Input.get_axis("forward", "backward")
		f_b_speed = Input.get_axis("left", "right")
	
	linear_velocity = Vector2(l_r_speed, f_b_speed).normalized().rotated(rotation - TAU) * SPEED

	
	
	get_node("Turret").rotation = rot
	
	if outputs[3] > 5.0:
		shoot()
	

func _input(event):
	if event.is_action_pressed("shoot"):
		shoot()
	
func damage():
	neural_net.score -= death_penalty
	print("i have been damaged")


func shoot():
	if $ReloadTimer.time_left > 0.01:
		return
	#$Turret/shoot_raycast.force_raycast_update()
	
	if $Turret/shoot_raycast.is_colliding():
		var collision_point = $Turret/shoot_raycast.get_collision_point()
		$Line2D.points[1] = collision_point - global_position
		
		var obj : Object = $Turret/shoot_raycast.get_collider()
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
