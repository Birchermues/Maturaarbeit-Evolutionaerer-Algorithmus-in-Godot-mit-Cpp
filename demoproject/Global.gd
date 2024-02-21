extends Node2D

@onready var player_scene = preload("res://Player.tscn")
@export var trainAI := false
@export var player_count := 1

var time:float = 0.0

@export var mut_chance : float
@export var weight_mut_strength : float
@export var bias_mut_strength : float

var player_arr : Array[CharacterBody2D] = []

var x : float = 0.0
var y : float = 0.0

func _ready():
	for i in player_count:
		get_node("/root/DinoRunner/Player Spawn").add_child(player_scene.instantiate())
	

	
func _process(delta):
	time += delta


func restart():
	print("ROUND OVER, NEXT ROUND STARTS")
	change_genes()
	time = 0.0;
	for obstacle in $"Obstacle Spawn".get_children():
		obstacle.queue_free()
		
	for player in $"Player Spawn".get_children():
		player.dead = false
		player.show()
		player.score = 0.0

	player_arr.clear()
	
	

func add_dead_player(player : CharacterBody2D):
	player_arr.append(player)
	if player_arr.size() == player_count:
		print(player_arr.size(), " of ", player_count, " players died")
		restart()
		
func change_genes():
	player_arr.sort_custom(custom_sort)
	print("best score: ", player_arr[0].score)
	for i in range(0, player_arr.size()):
		var player = player_arr[i]
		#print(player.score)
		var player_net : nn = player.get_node("nn")
		#player_net.w_and_b = player_arr[floor(float(player_count)/float(i)) - 1].get_node("nn").w_and_b
		player_net.w_and_b = player_arr[i % (player_count/3)].get_node("nn").w_and_b
		#Mutate
		if i > 3:
			player_net.mutate_w_and_b(mut_chance, weight_mut_strength * sqrt(i), bias_mut_strength * sqrt(i))
		

static func custom_sort(a : CharacterBody2D, b : CharacterBody2D):
	return (a.score > b.score)

