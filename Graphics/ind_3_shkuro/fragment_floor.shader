void main() {
if (mod(gl_FragCoord.x, 2*5) < 5)
float color1[]{ 1.0f, 1.0f, 1.0f, 1.0f };
float color2[]{ 1.0f, 0.0f, 1.0f, 1.0f };
gl_FragColor = color1;
else
gl_FragColor = color2;
}