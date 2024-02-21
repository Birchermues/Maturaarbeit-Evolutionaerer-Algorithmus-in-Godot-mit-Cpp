extends nn

@export var neural_network_res : NeuralNetworkRes;

var inputs : Array[float] = []
var w_and_b : Array[float];

# Called when the node enters the scene tree for the first time.
func _ready():
	if (neural_network_res.layout.size() < 3):
		push_error("must be more layers than 2")
	
	w_and_b = neural_network_res.weights_and_biases
	set_layers(neural_network_res.layout)
	fill_connections()
	match_weights_and_biases()
	
	
	#print(get_weights_and_biases())
	
	for i in range(get_layers()[get_layers().size()-1]):
		inputs.append(0.0)

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	var player_pos : Vector2 = get_parent().position

	
	var obstacles := get_node("/root/DinoRunner/Obstacle Spawn").get_children()
	
	var closest_obstacle : float = 100.0;
	for obstacle in obstacles:
		var distance :float = obstacle.position.x - player_pos.x
		if (distance < closest_obstacle):
			closest_obstacle = distance
			
	inputs[0] = player_pos.y
	inputs[1] = closest_obstacle

	var outputs : Array[float] = solve(inputs)
	
	#print("outputs: " + str(outputs))
	
	if outputs[0] > outputs[1]:
		get_parent().try_jump()

func match_weights_and_biases():
	var weights_and_biases_count : int = 0
	
	for i in range(get_layers().size() - 1):
		weights_and_biases_count += get_layers()[i] * get_layers()[i+1]
	
	#weights_and_biases_count *= 2;
	if weights_and_biases_count * 2 == w_and_b.size():
		print("amount of weights and biases provided equals the required amount: ", weights_and_biases_count*2, " are expected and ", w_and_b.size(), " are provided")
		set_weights_and_biases(w_and_b);
	else:
		print("amount of weights and biases does NOT equal the required amount: ", weights_and_biases_count*2, " are expected and ", w_and_b.size(), " are provided")
		print("weights and biases will be generated and randomized")
		randomize_weights_and_biases(weights_and_biases_count)


func randomize_weights_and_biases(weights_and_biases_count : int):
	w_and_b = []
	for i in range(weights_and_biases_count):
		w_and_b.append(randf_range(-0.5, 0.5))
		w_and_b.append(randf_range(-1, 1))
	
	#print(w_and_b)
	set_weights_and_biases(w_and_b)
