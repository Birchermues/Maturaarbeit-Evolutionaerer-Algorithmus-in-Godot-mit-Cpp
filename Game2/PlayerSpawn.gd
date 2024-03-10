extends ai_hub


@export var player_count : int = 10
@onready var player_scene : PackedScene = load("res://tank_player.tscn")

var player_nns : Array[nn] = []
var players : Array[Node] = []

@export var round_duration : float = 10.0

@export var neural_net_visualizer : TextureRect
# Called when the node enters the scene tree for the first time.
func _ready():
	$"../RoundTimer".wait_time = round_duration
	for i in range(player_count):
		var player := player_scene.instantiate()
		player.position = Vector2(randf_range(0, 6200), randf_range(0, 3600))
		add_child(player)
		player_nns.append(player.neural_net)
	set_nns(player_nns)
	players = get_children()
	#print(ai_controller.get_nns()[0].get_score())
	neural_net_visualizer.material.set_shader_parameter("w_and_b", get_nns()[0].get_weights_and_biases())


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	pass
	
	
func _on_round_timer_timeout():
	print("round has ended, new one will start")
	$"../RoundTimer".wait_time = round_duration
	
	sort_nns_on_score()
	
	
	var scores : String = "scores: "
	for i in range(get_nns().size()):
		
		players[i].position = Vector2(randf_range(0, 6200), randf_range(0, 3600))
		
		if i != 0:
			get_nns()[i].set_weights_and_biases(get_nns()[floor(i*i/player_count)].get_weights_and_biases())
			get_nns()[i].mutate(mut_chance, weight_mut_strength, bias_mut_strength)
		else:
			neural_net_visualizer.material.set_shader_parameter("w_and_b", get_nns()[0].get_weights_and_biases())

		scores += " / " + str(get_nns()[i].score)
		get_nns()[i].score = 0
	print(scores)
	
	
func custom_sort_func(a : nn, b : nn):
	return a.score < b.score
