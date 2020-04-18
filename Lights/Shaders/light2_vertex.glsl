#version 330 core

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;

out vec3 normal;
out vec3 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
    gl_Position = proj * view * model * vec4(vPos, 1.0);
    normal = vec3(view * model * vec4(vNormal, 0.0));   //Важно! У нормали нет гомогенной координаты w и аффинные преобразование к ней должны применяться, это означает, что чётвертый компонент должен равняться 0, иначе (если будет к примеру 1) - нормаль будет изменена неверно из-за того, что к её координатам будет применено ещё и перемещение. Таким образом на нормаль могут действовать только матрицы с неаффинным преобразованием (аля поворот, скейлинг, искажение).
    position = vec3(view * model * vec4(vPos, 1.0));
}
