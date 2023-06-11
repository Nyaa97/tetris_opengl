#version 330

in vec4 iccolor;
out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela


//uniform vec4 color=vec4(1,1,1,1);

void main(void) {
	pixelColor = iccolor;
}
