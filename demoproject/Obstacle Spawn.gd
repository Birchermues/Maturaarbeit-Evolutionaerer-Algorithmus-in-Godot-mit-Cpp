extends Node2D

@export var timer:Timer
@export var offset:float
@export var multiple_obstacles:bool = true

var times := 1

@onready var scene := load("res://Obstacle Dino Runner.tscn")
# Called when the node enters the scene tree for the first time.
func _ready():
	pass
	


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	if timer.is_stopped():
		pass


func _on_timer_timeout():
	var min_time := 0.8 + (times - 1) * 0.2
	
	timer.wait_time = randf_range(min_time, 1.5)
	
	times = 1
	
	if (multiple_obstacles):
		times = randi_range(1,3)
	
	for i in times:
		var instance = scene.instantiate()
		instance.position += Vector2(i * offset, 0)
		add_child(instance)

