extends RigidBody2D


@export var SPEED : float = 100.0
@export var ROTATION_SPEED : float= 10.0
@export var TRACK_WIDTH : float = 10.0

var right_speed : int = 0
var left_speed : int = 0

func _physics_process(delta):
	var W : bool = Input.is_action_pressed("forward")
	var S : bool = Input.is_action_pressed("backward")
	var A : bool = Input.is_action_pressed("left")
	var D : bool = Input.is_action_pressed("right")
	
	var r : int = 0
	var l : int = 0
	
	if A and S:
		r = -1
		l = 0
		
	elif D and S:
		r = 0
		l = -1
	elif A and W:
		r = 1
		l = 0
	elif D and W:
		r = 0
		l = 1
	elif S:
		r = -1
		l = -1
	elif W:
		r = 1
		l = 1
	elif D:
		r = -1
		l = 1
	elif A:
		r = 1
		l = -1
		
	var diff_l_r : int = l - r
	
	
	if diff_l_r == 0:
		pass
	
	var left_track_pos : Vector2 = global_position + TRACK_WIDTH * -Vector2(cos(rotation), sin(rotation))
	var right_track_pos : Vector2 = global_position + TRACK_WIDTH * Vector2(cos(rotation), sin(rotation))
	
	apply_force(SPEED * l * Vector2(cos(rotation + TAU), sin(rotation+TAU)), left_track_pos)
	apply_force(SPEED * r * Vector2(cos(rotation + TAU), sin(rotation+TAU)), right_track_pos)
	
	print(rotation)
	
	
	
	set_move_input(Input.get_axis("ui_down", "ui_up"), Input.get_axis("ui_down", "ui_up"))

	
	
func rotate_move_tank(rotation_speed : float, rotation_distance : float):
	var rotation_point : Vector2 = global_position + TRACK_WIDTH * rotation_distance * Vector2(cos(rotation), sin(rotation))
	var x : float = rotation + rotation_speed
	global_position = rotation_point + Vector2(acos(x), asin(x)) * rotation_distance
	rotation = x
	
	
	
	
	
func set_move_input(right_track_speed : int, left_track_speed : int):
	right_speed = clampi(right_track_speed, -1, 1)
	left_speed = clampi(left_track_speed, -1, 1)
	

