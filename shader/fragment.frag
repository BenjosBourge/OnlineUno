uniform sampler2D texture;
uniform vec3 uColor;
uniform vec3 uColorOverlay;
uniform float uOverlay;
uniform vec3 uColorMask;
uniform float uColorMaskOpacity;
uniform float uOpacity;

void main() {
    vec4 color = texture2D(texture, gl_TexCoord[0].xy);
    color.rgb = color.rgb * uColor;
    color.rgb = mix(color.rgb, uColorMask, uColorMaskOpacity);

    float overlay = max(0., uOverlay - color.a);

    vec2 texOffset = 1.0 / textureSize(texture, 0);
    float borderValue = texture2D(texture, gl_TexCoord[0].xy + vec2(0, texOffset.y)).a;
    borderValue += texture2D(texture, gl_TexCoord[0].xy + vec2(0, -texOffset.y)).a;
    borderValue += texture2D(texture, gl_TexCoord[0].xy + vec2(texOffset.x, 0)).a;
    borderValue += texture2D(texture, gl_TexCoord[0].xy + vec2(-texOffset.x, 0)).a;
    borderValue = min(1., borderValue);
    overlay = min(overlay, borderValue);

    color.rgb = mix(color.rgb, uColorOverlay, overlay);
    color.a = max(overlay, color.a);
    color.a = color.a * uOpacity;
    gl_FragColor = color;
}