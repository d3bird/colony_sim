attribute vec4 vPosition;
attribute vec4 vColor;
varying vec4 color;

uniform mat4 Projection;
uniform mat4 model_trans;
uniform mat4 model_view;
void main() 
{

vec4 temp;
  
  temp.x = (1.0/1000.0)*(vPosition.x );
  temp.y = (1.0/1000.0)*(vPosition.y );
  temp.z = (1.0/1000.0)*(vPosition.z );
  temp.w =1;


  //gl_Position =Projection* model_view  * ((temp ));
  gl_Position =Projection* model_view * model_trans * ((temp ));


  color = vColor;
} 
