#version 440

uniform sampler2D refractionBackground;
uniform sampler2D refractionOverlay;
uniform sampler2D dropAlpha;
uniform sampler2D dropColor;
uniform sampler2D dropShine;

layout(location = 0)in vec4 texC;
layout(location = 0)out vec4 fragColor;

uniform vec2 dropLocation; //imGesamtbild
uniform int dropRadius;
uniform int planeWidth;
uniform int planeHeight;

vec2 scaledLocation(vec2 nonScaledLocation) {
    return vec2(nonScaledLocation.x / float(planeWidth), nonScaledLocation.y / float(planeHeight));
}

vec4 blend(vec4 bg, vec4 fg){
  vec3 bgm = bg.rgb*bg.a;
  vec3 fgm = fg.rgb*fg.a;
  float ia = 1.0 - fg.a;
  float a = (fg.a + bg.a * ia);
  vec3 rgb;
  if(a != 0.0) {
    rgb = (fgm + bgm * ia) / a;
  }
  else {
    rgb=vec3(0.0,0.0,0.0);
  }
  return vec4(rgb,a);
}


//Verrechnet dropAlpha mit refractionColor an dieser Stelle
vec4 applyAlpha(vec4 opaqueColor) {
    vec4 alpha = texture(dropAlpha, texC.xy);
    opaqueColor.a = clamp(alpha.a*20 - 5, 0.0, 1.0);
    return opaqueColor;
}

vec4 getRefractionColor(vec2 refraction) {

    //dieser Wert ist jetzt normalisiert auf Tropfenkoordinaten
    vec2 scaledDropPosition = scaledLocation(dropLocation); //Pos tropfen auf Background in 0-1 koords

    vec2 scaledRefPos = scaledLocation(refraction*2.5*dropRadius); //textur koordinaten skaliert auf
    vec2 backgroundTexLookUpPos = scaledDropPosition + scaledRefPos;
    backgroundTexLookUpPos = clamp(backgroundTexLookUpPos, 0.0, 1.0);
    vec4 originalBackgroundColor = texture(refractionBackground, backgroundTexLookUpPos);
    vec4 originalOverlayColor = texture(refractionOverlay, backgroundTexLookUpPos);
    vec4 mixed = mix(originalBackgroundColor, originalOverlayColor, 0.05f);
    //vec4 shineColor = texture(dropShine, texC.xy);
    return mixed;
}

vec2 getRefraction() {
    //y Pos = green
    //x Pos = red
    //(z Pos = blue) von mir nicht genutzt

    //positionen anhand der Farbkanäle flippen
    vec4 texColor = texture(dropColor, texC.xy);
    vec2 newTexPos = vec2((texColor.g / 1.f), (texColor.r / 1.f));
    return (newTexPos-0.5)*2;
}

void main() {
    vec2 refraction = getRefraction();
    vec4 refractionColor = applyAlpha(getRefractionColor(refraction));

    //Billig Schatten
    //refractionColor = refractionColor - vec4(0.5, 0.5, 0.5, 0.5) * ( 1 - refractionColor.a);
    fragColor = refractionColor;
}
