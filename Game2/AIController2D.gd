extends AIController2D


@onready var sensor : RaycastSensor2D = $"../RaycastSensor2D"

func _physics_process(delta):
	if needs_reset:
		reset()
		return
		
# Called when the node enters the scene tree for the first time.
func get_obs() -> Dictionary:
	var sensors : Array = sensor.calculate_raycasts()
	for i in sensors:
		i *= 0.01
	var observations = [get_parent().position.x, get_parent().position.y, $"../ReloadTimer".time_left]
	return {"obs": sensors + observations}

func get_reward() -> float:
	return get_parent().neural_net.score
	
func get_action_space() -> Dictionary:
	return {
		"move" : {
			"size": 2,
			"action_type": "continuous"
		},
		"shoot" : {
			"size": 1,
			"action_type": "discrete" 
		},
		"rotate" : {
			"size": 1,
			"action_type": "continuous"
		}
	}


func set_action(action) -> void:
	var x = action["move"][0]
	var y = action["move"][1]
	
	get_parent().move(x, y)
	
	print(action["move"][0])
	
	$"../Turret".rotation = action["rotate"][0] * PI
	
