void main() {
    vec4 sum;
    for (int i = 1; i != 2; i += 2) {
        sum += vec4(0.1, 0.1, 0.1, 0.1);
    }
    gl_FragColor = sum;
}
