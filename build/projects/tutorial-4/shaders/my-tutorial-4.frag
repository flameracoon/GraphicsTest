 #version 450 core
	//Get vector color from vertex
	uniform vec3 uColor;
	//Output color
	layout(location=0)out vec4 fFragColor;
	void main(){
	 fFragColor=vec4(uColor, 1.0);
	}
