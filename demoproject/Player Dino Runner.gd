extends CharacterBody2D

const SPEED = 300.0
const JUMP_VELOCITY = -900.0
@onready var dino_runner = $".."
var dead : bool = false

# Get the gravity from the project settings to be synced with RigidBody nodes.
var gravity = ProjectSettings.get_setting("physics/2d/default_gravity") * 2

func _physics_process(delta):
	# Add the gravity.
	if not is_on_floor():
		velocity.y += gravity * delta

	# Handle jump.
	if Input.is_action_just_pressed("ui_accept"):
		try_jump()

	# Get the input direction and handle the movement/deceleration.
	# As good practice, you should replace UI actions with custom gameplay actions.
	var direction = Input.get_axis("ui_left", "ui_right")
	if direction:
		velocity.x = direction * SPEED
	else:
		velocity.x = move_toward(velocity.x, 0, SPEED)

	move_and_slide()
	
func kill():
	dead = true
	dino_runner = get_tree().root.get_child(0)
	print("Player died with score " + str(dino_runner.score))
	hide()
	
func try_jump():
	if is_on_floor():
		velocity.y = JUMP_VELOCITY
