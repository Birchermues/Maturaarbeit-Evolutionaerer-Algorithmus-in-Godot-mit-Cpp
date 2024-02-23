extends ai_hub


# Called when the node enters the scene tree for the first time.
func _ready():
	set_mut_chance(234.43)
	print(get_mut_chance())


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	pass
