extends RichTextLabel

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	var global = $".."
	var score_rounded = floor(global.time)
	text = "Score: " + str(score_rounded)
