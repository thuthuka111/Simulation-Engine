#version 330 core

out vec4 fragColor;

// in vec3 ourColor;
// in vec2 texCoord;

// uniform sampler2D texture1;
// uniform sampler2D texture2;

void main() {
    //fragColor = texture(outTexture, texCoord) * vec4(ourColor, 1.0);
    // fragColor = texture(texture1, texCoord);
    // fragColor = mix(texture(texture1, texCoord), texture(texture2, texCoord), 0.2);
    fragColor = vec4(0.0f, 0.156f, 1.0f, 1.0f);
}