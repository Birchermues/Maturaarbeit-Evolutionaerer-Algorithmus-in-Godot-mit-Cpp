# baut auf dem c++ skript ai_hub auf und ist eine instanz im spiel
extends ai_hub

# anzahl ki gesteuerter spieler
@export var player_count : int = 10
# der spieler
@onready var player_scene : PackedScene = load("res://tank_player.tscn")

# referenz zur instanz des spielers und zu einem neuronalen netz pro spieler
var player_nns : Array[nn] = []
var players : Array[Node] = []

@export var round_duration : float = 10.0

# für die visualisierung der gewichte
@export var neural_net_visualizer : TextureRect

func _ready():
	$"../RoundTimer".wait_time = round_duration
	# alle spieler werden instanziiert und ein neuronales netz wird hinzugefügt
	for i in range(player_count):
		var player := player_scene.instantiate()
		player.position = Vector2(randf_range(0, 6200), randf_range(0, 3600))
		add_child(player)
		player_nns.append(player.neural_net)
	set_nns(player_nns)
	players = get_children()
	neural_net_visualizer.material.set_shader_parameter("w_and_b", get_nns()[0].get_weights_and_biases())


func _on_round_timer_timeout():
	print("round has ended, new one will start")
	$"../RoundTimer".wait_time = round_duration
	
	# die neuronalen netze der spieler werden in reihenfolge ihrer scores gebracht
	sort_nns_on_score()

	# zur darstellung der score-verteilung
	var scores : String = "scores: "
	var avg_score : float = 0.0
	
	# das neuronale netzwerk des besten spielers visualisieren
	neural_net_visualizer.material.set_shader_parameter("w_and_b", get_nns()[0].get_weights_and_biases())
	
	#liste mit allen neuen serialisierten gewichten und biases des neuronalen netzes erstellen
	var new_w_and_b = []
	for i in range(get_nns().size()):
		# genetische elite wird überproportional in der nächsten generation vertreten
		new_w_and_b.append(get_nns()[floor((i*i)/player_count)].get_weights_and_biases())
		
		# scores anzeigen
		scores += " / " + str(get_nns()[i].score)
		avg_score += get_nns()[i].score
		
		# reset
		get_nns()[i].score = 0
		players[i].position = Vector2(randf_range(0, 6200), randf_range(0, 3600))
	
	# scores anzeigen
	print(scores)
	print("Average Score: " + str(avg_score/player_count))

	for i in range(new_w_and_b.size()):
		# die gewichte werden neu verteilt
		get_nns()[i].set_weights_and_biases(new_w_and_b[i])
		# mutation
		get_nns()[i].mutate(mut_chance, mut_strength) 
	

# funktion mit der die neuronalen netze der spieler nach Score sortiert werden
func custom_sort_func(a : nn, b : nn):
	return a.score > b.score
