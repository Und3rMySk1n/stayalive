#version 130
// GLSL version 130 enabled with OpenGL Core Profile 3.0.
//  - `varying` renamed to `in` for fragment shader

struct LightSource
{
    // (x, y, z, 1) means positioned light.
    // (x, y, z, 0) means directed light.
    vec4 position;
    vec4 diffuse;
    vec4 specular;
    vec3 direction;
    float cutoff;
};

struct LightFactors
{
    float diffuse;
    float specular;
};

const float SHININESS = 100.0;

uniform LightSource light0;
uniform sampler2D colormap;
uniform mat4 view;

in vec2 fragTextureUV;
in vec3 fragNormal;
in vec3 fragViewDirection;

void main()
{
    // Get base color by fetching the texture
    vec4 color = texture2D(colormap, fragTextureUV.st);
    vec4 lightPosInViewSpace = view * light0.position;


    // start calculating spotlight effect
    
    float attenuation = 1.0;
    
    vec3 viewDirection = normalize(-fragViewDirection);
    vec3 delta = light0.position.w * viewDirection;
    vec3 lightDirection = normalize(lightPosInViewSpace.xyz + delta);
    
    float clampedCosine = dot(-lightDirection, normalize(light0.direction));
    if (clampedCosine < cos(light0.cutoff * 3.14159 / 180.0)) // outside of spotlight cone
    {
        attenuation = 0.7;
    }
    
    color = color * attenuation;    
    // end calculating spotlight effect


    gl_FragColor = color;
}
