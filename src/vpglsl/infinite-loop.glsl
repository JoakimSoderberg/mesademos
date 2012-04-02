void main() {
   gl_Position = gl_Vertex;
   vec4 sum = vec4(0);
   for (int i = 1; i != 2; i += 2) {
      sum += vec4(0.1, 0.1, 0.1, 0.1);
   }
   gl_FrontColor = sum;
}
