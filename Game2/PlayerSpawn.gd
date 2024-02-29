extends Node2D


@export var player_count : int = 10
@onready var player_scene : PackedScene = load("res://tank_player.tscn")

var ai_controller : ai_hub = ai_hub.new()

# Called when the node enters the scene tree for the first time.
func _ready():
	var player_nns : Array[nn] = []
	for i in range(player_count):
		var player = player_scene.instantiate()
		player.position = Vector2(randf_range(0, 6200), randf_range(0, 3600))
		add_child(player)
		player_nns.append(player.neural_net)
	ai_controller.set_nns(player_nns)
	ai_controller.sort_nns_on_score()
	#print(ai_controller.get_nns()[0].get_score())


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	pass
