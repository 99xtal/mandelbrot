#version 330 core

out vec4 FragColor;
float maxIterations = 100.0f;

vec2 squareImaginary(vec2 number) {
	return vec2(
		pow(number.x,2)-pow(number.y,2),
		2*number.x*number.y
	);
}

float iterateMandelbrot(vec2 coord) {
	vec2 z = vec2(0,0);
	for(int i=0;i<maxIterations;i++){
		z = squareImaginary(z) + coord;
		if(length(z)>2) return i/maxIterations;
	}
	return maxIterations;
}

vec2 getCoordinatesFromScreen(vec2 fragCoord, vec2 vSystemResolution, vec4 coordinateRange){
	vec2 normalizedCoordinates = gl_FragCoord.xy/vSystemResolution; //From 0 to 1 where pixel is in screen

	float horizontalSize = coordinateRange.y-coordinateRange.x; //Size of horizontal part of screen
	float verticalSize = coordinateRange.w-coordinateRange.z; //Size of vertical part of screen
	
	normalizedCoordinates.x = normalizedCoordinates.x*horizontalSize+coordinateRange.x; //Multiply by size and add initial offset position
	normalizedCoordinates.y = normalizedCoordinates.y*verticalSize+coordinateRange.z;

	return normalizedCoordinates;
}

void main() {
    vec2 resolution = vec2(800.0f, 800.0f);
    vec2 normalizedCoords = getCoordinatesFromScreen(gl_FragCoord.xy, resolution, vec4(-2.5, 2.5, -2.5, 2.5));
    float iters = iterateMandelbrot(normalizedCoords);
    FragColor = vec4(iters); 
}