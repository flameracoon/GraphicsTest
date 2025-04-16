
#version 450 core
	//Set location variables
	layout(location=0) in vec2 aVertexPosition;

	uniform mat3 uModel_to_NDC;
	void main(){
		//Set position, Z is 0 for now
		gl_Position=vec4(vec2(uModel_to_NDC*vec3(aVertexPosition, 1.f)),
        0.0, 1.0);

	}
