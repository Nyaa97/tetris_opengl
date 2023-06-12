#version 330 core

// Ouput data
//layout(location = 0) out float fragmentdepth;

out vec4 pixelColor;

void main(){
    // Not really needed, OpenGL does it anyway
    //fragmentdepth = gl_FragCoord.z;
    pixelColor = vec4( vec3( gl_FragCoord ), 1 );
}
