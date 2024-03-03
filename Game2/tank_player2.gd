extends RigidBody2D

@export var SPEED : float = 100.0
@export var amount_of_raycasts : int = 11
@export var raycast_fov : float = 180
@export var raycast_dist : float = 1000

@export var raycasts_rotate_with_turret : bool = false

var f_b_speed : float = 0.0
var l_r_speed : float = 0.0

@onready var neural_net : nn = nn.new()

# Called when the node enters the scene tree for the first time.
func _ready():
	$Line2D.visible = false
	
	for i in range(amount_of_raycasts):
		var ray : RayCast2D = RayCast2D.new()
		#var line : Line2D = Line2D.new()
		
		var rot : float = raycast_fov / -2.0 + (i * raycast_fov / (amount_of_raycasts - 1))
		ray.target_position = Vector2(cos(deg_to_rad(rot)), sin(deg_to_rad(rot))) * raycast_dist
		#line.add_point(Vector2(0, 0), 0)
		#line.add_point(Vector2(cos(deg_to_rad(rot)), sin(deg_to_rad(rot))) * raycast_dist, 0)
		ray.set_collision_mask_value(2, true)
		if raycasts_rotate_with_turret:
			get_node("Turret").add_child(ray)
		else:
			add_child(ray)
	
	neural_net.


func _physics_process(delta):
	f_b_speed = Input.get_axis("forward", "backward")
	l_r_speed = Input.get_axis("left", "right")
	
	linear_velocity = Vector2(l_r_speed, f_b_speed).normalized().rotated(rotation - TAU) * SPEED
	var vec_to_mouse = get_global_mouse_position() - global_position
	
	
	get_node("Turret").rotation = atan2(vec_to_mouse.y, vec_to_mouse.x)
	

func _input(event):
	if event.is_action_pressed("shoot"):
		shoot()
	
func damage():
	print("i have been damaged")


func shoot():
	$Turret/shoot_raycast.force_raycast_update()
	
	if $Turret/shoot_raycast.is_colliding():
		var collision_point = $Turret/shoot_raycast.get_collision_point()
		$Line2D.points[1] = collision_point - global_position
		
		var obj : Object = $Turret/shoot_raycast.get_collider()
		if obj.get_node(".").get_class() == "RigidBody2D":
			print("hit a player")
			obj.damage()
		
	else:
		$Line2D.points[1] = Vector2(cos($Turret.rotation), sin($Turret.rotation)) * 3000 #+ $Turret.global_position
	$Line2D.visible = true
	$Line2D/Timer.start()
	
	



func _on_timer_timeout():
	$Line2D.visible = false
