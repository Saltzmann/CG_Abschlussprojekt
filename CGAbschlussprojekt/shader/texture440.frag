#version 440

uniform sampler2D diffuseMap;
uniform sampler2D blendMap;

layout(location = 0)in vec4 texC;
layout(location = 0)out vec4 fragColor;

const float blur = 00.0f; //blur by X Pixel
const float hstep = 2.0f; //blur entlang x Achse
const float vstep = 2.0f; //blur entlang y Achse

/* Funktioniert leider noch nicht ganz
vec4 blurredTexture() {
    vec2 tc = texC.xy;
    vec4 sum = vec4(0.f);

   //apply blurring, using a 9-tap filter with predefined gaussian weights
   sum += texture(diffuseMap, vec2(tc.x - 4.0*blur*hstep, tc.y - 4.0*blur*vstep)) * 0.0162162162;
   sum += texture(diffuseMap, vec2(tc.x - 3.0*blur*hstep, tc.y - 3.0*blur*vstep)) * 0.0540540541;
   sum += texture(diffuseMap, vec2(tc.x - 2.0*blur*hstep, tc.y - 2.0*blur*vstep)) * 0.1216216216;
   sum += texture(diffuseMap, vec2(tc.x - 1.0*blur*hstep, tc.y - 1.0*blur*vstep)) * 0.1945945946;

   sum += texture(diffuseMap, vec2(tc.x, tc.y)) * 0.2270270270;

   sum += texture(diffuseMap, vec2(tc.x + 1.0*blur*hstep, tc.y + 1.0*blur*vstep)) * 0.1945945946;
   sum += texture(diffuseMap, vec2(tc.x + 2.0*blur*hstep, tc.y + 2.0*blur*vstep)) * 0.1216216216;
   sum += texture(diffuseMap, vec2(tc.x + 3.0*blur*hstep, tc.y + 3.0*blur*vstep)) * 0.0540540541;
   sum += texture(diffuseMap, vec2(tc.x + 4.0*blur*hstep, tc.y + 4.0*blur*vstep)) * 0.0162162162;

   return vec4(sum.rgb, 1.0);
}
*/
void main() {
    //vec4 tex1 = blurredTexture();
    float darken = 0.4;
    vec4 tex1 = texture(diffuseMap, texC.xy) - vec4(darken);
    vec4 tex2 = texture(blendMap, texC.xy);
    fragColor = mix(tex1, tex2, 0.3f);
}
