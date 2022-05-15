#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

layout(location = 1) in vec3 aNormal;

layout(location = 2) in vec2 texCoord;  
//只给了对应顶点的纹理坐标，剩下其他的像素如何着色，需要用到顶点插值的方法，games101讲了，当纹理像素少的时候，可能需要用到线性插值(双线性插值)，
//当纹理像素多的时候，会用到Mipmap(其中的平滑过度会用到三线性插值)，或者Anisotropic(各向异性过滤)等。


out vec2 _TexCoord;
out vec3 _Normal;
out vec3 _FragPos; //shading point位置

uniform mat4 transform;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
    _TexCoord = texCoord;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;
    //在世界空间坐标系下的各个片段的位置向量
    _FragPos = vec3(modelMatrix * position);

    _Normal = normalize(mat3(transpose(inverse(modelMatrix))) * aNormal);
};

#shader fragment
#version 330 core

//layout(location = 0) out vec4 color;

in vec2 _TexCoord;
in vec3 _Normal;
in vec3 _FragPos;

struct Material {
    vec3 s_Ambient;
    sampler2D s_Diffuse;
    sampler2D s_Specular;
    float s_Shininess;
};

uniform vec3 u_Color;
uniform vec3 u_Ambient;
uniform vec3 u_LightPos;
uniform vec3 u_LightColor;
uniform vec3 u_CameraPos;

uniform Material u_Material;
uniform sampler2D u_Texture;


out vec4 FragColor;

void main()
{
    //vec4 texColor = texture(u_Texture, _TexCoord);

    vec3 lightDirection = normalize(u_LightPos - _FragPos);

    //GLSL提供一种方法，求光镜面反射向量(第一个参数：光的入射方(因为求出光的反射向量，所以要加个 - 号)), 第二个参数是法线)。
    vec3 specularReflectDirection = reflect(-lightDirection, _Normal);
    vec3 cameraDirection = normalize(u_CameraPos - _FragPos); //入射向量是，摄像机的位置看向Fragment片段，也就是shading point位置。

    float specularAngle = pow(max(dot(specularReflectDirection, cameraDirection), 0), u_Material.s_Shininess); //pow:指数型衰减，角度越小，镜面反射越明显
    
    //高光
    vec3 specular = texture(u_Material.s_Specular, _TexCoord).rgb * specularAngle * u_LightColor;

    //漫反射
    //vec3 diffuse = texture(u_Material.s_Diffuse, _TexCoord).rgb;
    vec3 diffuse = texture(u_Material.s_Diffuse, _TexCoord).rgb * max(dot(lightDirection, _Normal), 0) * u_LightColor;

    //环境光
    //vec3 ambient = u_Material.s_Ambient * u_Ambient;
    vec3 ambient = u_Material.s_Ambient * u_Ambient * texture(u_Material.s_Diffuse, _TexCoord).rgb; //环境光也加上了纹理素材


    FragColor = vec4((diffuse + ambient + specular) * u_Color, 1.0);

    //color = u_Color * u_Ambient;
    //vec4 FragColor = mix(texture(texture1, _TexCoord), texture(texture2, _TexCoord), 0.2);
};





































