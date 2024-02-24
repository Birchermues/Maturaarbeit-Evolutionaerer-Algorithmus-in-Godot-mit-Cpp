extends RigidBody2D

@export var SPEED : float = 100.0
@export var amount_of_raycasts : int = 11
@export var raycast_fov : float = 180
@export var raycast_dist : float = 1000

var f_b_speed : float = 0.0
var l_r_speed : float = 0.0

# Called when the node enters the scene tree for the first time.
func _ready():
	for i in range(amount_of_raycasts):
		var ray : RayCast2D = RayCast2D.new()
		#var line : Line2D = Line2D.new()
		
		var rot : float = raycast_fov / -2.0 + (i * raycast_fov / (amount_of_raycasts - 1))
		ray.target_position = Vector2(cos(deg_to_rad(rot)), sin(deg_to_rad(rot))) * raycast_dist
		#line.add_point(Vector2(0, 0), 0)
		#line.add_point(Vector2(cos(deg_to_rad(rot)), sin(deg_to_rad(rot))) * raycast_dist, 0)
		get_node("Turret").add_child(ray)
		#get_node("Turret").add_child(line)
		print("iosduhfg")



func _physics_process(delta):
	f_b_speed = Input.get_axis("forward", "backward")
	l_r_speed = Input.get_axis("left", "right")
	
	linear_velocity = Vector2(l_r_speed, f_b_speed).normalized().rotated(rotation - TAU) * SPEED
	var vec_to_mouse = get_global_mouse_position() - global_position
	
	
	get_node("Turret").rotation = atan2(vec_to_mouse.y, vec_to_mouse.x)
