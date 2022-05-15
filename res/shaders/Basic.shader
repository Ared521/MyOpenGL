#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

layout(location = 1) in vec3 aNormal;

layout(location = 2) in vec2 texCoord;  
//ֻ���˶�Ӧ������������꣬ʣ�����������������ɫ����Ҫ�õ������ֵ�ķ�����games101���ˣ������������ٵ�ʱ�򣬿�����Ҫ�õ����Բ�ֵ(˫���Բ�ֵ)��
//���������ض��ʱ�򣬻��õ�Mipmap(���е�ƽ�����Ȼ��õ������Բ�ֵ)������Anisotropic(�������Թ���)�ȡ�


out vec2 _TexCoord;
out vec3 _Normal;
out vec3 _FragPos; //shading pointλ��

uniform mat4 transform;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
    _TexCoord = texCoord;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;
    //������ռ�����ϵ�µĸ���Ƭ�ε�λ������
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

    //GLSL�ṩһ�ַ�������⾵�淴������(��һ��������������䷽(��Ϊ�����ķ�������������Ҫ�Ӹ� - ��)), �ڶ��������Ƿ���)��
    vec3 specularReflectDirection = reflect(-lightDirection, _Normal);
    vec3 cameraDirection = normalize(u_CameraPos - _FragPos); //���������ǣ��������λ�ÿ���FragmentƬ�Σ�Ҳ����shading pointλ�á�

    float specularAngle = pow(max(dot(specularReflectDirection, cameraDirection), 0), u_Material.s_Shininess); //pow:ָ����˥�����Ƕ�ԽС�����淴��Խ����
    
    //�߹�
    vec3 specular = texture(u_Material.s_Specular, _TexCoord).rgb * specularAngle * u_LightColor;

    //������
    //vec3 diffuse = texture(u_Material.s_Diffuse, _TexCoord).rgb;
    vec3 diffuse = texture(u_Material.s_Diffuse, _TexCoord).rgb * max(dot(lightDirection, _Normal), 0) * u_LightColor;

    //������
    //vec3 ambient = u_Material.s_Ambient * u_Ambient;
    vec3 ambient = u_Material.s_Ambient * u_Ambient * texture(u_Material.s_Diffuse, _TexCoord).rgb; //������Ҳ�����������ز�


    FragColor = vec4((diffuse + ambient + specular) * u_Color, 1.0);

    //color = u_Color * u_Ambient;
    //vec4 FragColor = mix(texture(texture1, _TexCoord), texture(texture2, _TexCoord), 0.2);
};





































