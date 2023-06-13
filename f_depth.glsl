#version 420

layout(location = 0) out float fragDepth;
out vec4 pixelColor;

void main() {
    fragDepth = gl_FragCoord.z;
    pixelColor = vec4( vec3( gl_FragCoord.z ), 1 );
}
