extends nn

@export var neural_network_res : NeuralNetworkRes;

var inputs : Array[float] = []

# Called when the node enters the scene tree for the first time.
func _ready():
	if (neural_network_res.layout.size() < 3):
		push_error("must be more layers than 2")
	
	
	var w_and_b : Array[float] = neural_network_res.weights_and_biases
	set_weights_and_biases(w_and_b);
	layer_sizes = neural_network_res.layout
	#print(get_weights_and_biases())
	
	for i in range(layer_sizes[layer_sizes.size()-1]):
		inputs.append(0.0)

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	var player_pos : Vector2 = get_parent().position
	inputs[0] = (player_pos.y)
	
	var obstacles := get_node("/root/DinoRunner/Obstacle Spawn").get_children()
	
	var closest_obstacle : float = 100.0;
	for obstacle in obstacles:
		var distance :float = obstacle.position.x - player_pos.x
		if (distance < closest_obstacle):
			closest_obstacle = distance
	
	inputs[1] = closest_obstacle

	var outputs : Array[float] = solve(inputs)
	
	#print("outputs: " + str(outputs))
