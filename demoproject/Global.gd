extends Node2D

@onready var player_scene = preload("res://Player.tscn")
@export var trainAI := false
@export var player_count := 1

var time:float = 0.0

@export var mut_chance : float
@export var weight_mut_strength : float
@export var bias_mut_strength : float

var player_arr : Array[CharacterBody2D] = []

func _ready():
	for i in player_count:
		get_node("/root/DinoRunner/Player Spawn").add_child(player_scene.instantiate())
		
func _process(delta):
	time += delta

func restart():
	print("ROUND OVER, NEXT ROUND STARTS")
	time = 0.0;
	for obstacle in $"Obstacle Spawn".get_children():
		obstacle.queue_free()
		
	for player in $"Player Spawn".get_children():
		player.dead = false
		player.show()
		player.score = 0.0
		
	change_genes()
	player_arr.clear()
	
	

func add_dead_player(player : CharacterBody2D):
	player_arr.append(player)
	if player_arr.size() == player_count:
		print(player_arr.size(), " of ", player_count, " players died")
		restart()
		
func change_genes():
	for i in range(player_arr.size() - 5):
		var player = player_arr[i]
		var player_net : nn = player.get_node("nn")
		player_net.w_and_b = player_arr[-1].get_node("nn").w_and_b
		#Mutate
		player_net.mutate_w_and_b(mut_chance, weight_mut_strength, bias_mut_strength)
		





