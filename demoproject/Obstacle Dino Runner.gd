extends RigidBody2D

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	var vec = Vector2(-500.0, 0.0)
	#position += vec * delta
	#linear_velocity = vec
	move_and_collide(vec * delta)
	
	if (global_position.x < -200.0):
		queue_free()


func _on_area_2d_body_entered(body):
	body.kill()
