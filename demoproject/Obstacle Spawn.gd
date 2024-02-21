extends Node2D

@export var timer:Timer
@export var offset:float
@export var multiple_obstacles:bool = true

@export var high_obstacle_height : float
@export var high_obstacle_chance : float = 0.0

var times := 1
var is_high : bool = false

@onready var scene := load("res://Obstacle Dino Runner.tscn")
# Called when the node enters the scene tree for the first time.
func _ready():
	pass
	


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	if timer.is_stopped():
		pass


func _on_timer_timeout():
	var min_time : float= 1.0 + (times - 1) * 0.2
	
	var is_next_high : bool = false
	if randf() < high_obstacle_chance:
		is_next_high = true
	
	if is_high != is_next_high:
		min_time -= 0.6
	
	timer.wait_time = randf_range(min_time, 2.2)
	
	times = 1
	
	if (multiple_obstacles):
		times = randi_range(1,3)
	
	for i in times:
		var instance = scene.instantiate()
		var height : float = float(is_high) * high_obstacle_height
		instance.position += Vector2(i * offset, height)
		add_child(instance)
		
	is_high = is_next_high

