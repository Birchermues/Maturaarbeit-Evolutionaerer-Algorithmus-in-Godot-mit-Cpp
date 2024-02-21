extends CharacterBody2D

const SPEED = 300.0
const JUMP_VELOCITY = -900.0
@onready var global = $".."
var dead : bool = false
var score : float = 0.0

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
	if not dead:
		dead = true
		global = get_tree().root.get_child(0)
		score = global.time
		global.add_dead_player(self)
		#print("Player died with score " + str(score))
		hide()
	
func try_jump():
	if is_on_floor():
		velocity.y = JUMP_VELOCITY
		return true
	return false
	
