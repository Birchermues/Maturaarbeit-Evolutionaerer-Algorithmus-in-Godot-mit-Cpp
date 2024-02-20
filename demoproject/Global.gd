extends Node2D

@onready var player = preload("res://Player.tscn")

@export var trainAI := false
@export var player_count := 1

@export var score:float = 0.0


func _ready():
	for i in player_count:
		get_node("/root/DinoRunner/Player Spawn").add_child(player.instantiate())


func _process(delta):
	pass

func get_score():
	return score




