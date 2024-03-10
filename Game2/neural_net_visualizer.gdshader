shader_type canvas_item;

uniform float[4096] w_and_b;


void vertex() {
	// Called for every vertex the material is visible on.
}

void fragment() {
	int id = 0;
	int x = int(floor(UV.x *64.0));
	int y = int(floor(UV.y * 64.0));
	
	id = x * 64 + y;
	float value;

	value = (w_and_b[id] + 3.0) * 0.16666;
	
	vec4 col1 = vec4(1.0, 1.0, 0.0, 1.0);
	vec4 col2 = vec4(1.0, 0.0, 0.0, 1.0);
	
	
	COLOR = mix(col1, col2, value);
}

//void light() {
	// Called for every pixel for every light affecting the CanvasItem.
	// Uncomment to replace the default light processing function with this one.
//}
