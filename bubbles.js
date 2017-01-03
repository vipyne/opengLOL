var MAX_SPHERES = 100;
var squareVertexPositionBuffer;
var mvMatrix = mat4.create();
var pMatrix = mat4.create();
var shaderProgram;
var vertices = [];
var theta = 0;
var lastTime = 0;
var data = [];
var textureSize = MAX_SPHERES * 3;
var numSpheres = 50;
var textureId = 0;
var gl;

class SphereHelper
{
   constructor()
   {
      this.pos = [0.0,0.0,0.0];
      this.vel = [0.0,0.0,0.0];
      this.rgb = [0.0,0.0,0.0];
      this.radius = 0.0;
   }

   fromData(idx)
   {
     var i = idx * 3 * 4;
     this.pos[0] = data[i + 0];
     this.pos[1] = data[i + 1];
     this.pos[2] = data[i + 2];
     this.radius = data[i + 3];

     this.vel[0] = data[i + 4];
     this.vel[1] = data[i + 5];
     this.vel[2] = data[i + 6];

     this.rgb[0] = data[i + 8];
     this.rgb[1] = data[i + 9];
     this.rgb[2] = data[i + 10];
   }

   toData(sphereIndex)
   {
     var i = sphereIndex * 3 * 4;

     // pos
     data[i + 0] = this.pos[0]; // x
     data[i + 1] = this.pos[1]; // y
     data[i + 2] = this.pos[2]; // z
     data[i + 3] = this.radius; // radius
     // velocity
     data[i + 4] = this.vel[0]; // v x
     data[i + 5] = this.vel[1]; // v y
     data[i + 6] = this.vel[2]; // v z
     data[i + 7] = 0.0; // free for now
     // color
     data[i + 8] = this.rgb[0]; // r
     data[i + 9] = this.rgb[1]; // g
     data[i + 10] = this.rgb[2]; // b
     data[i + 11] = 1.0; // free for now      
   }

   update(idx, dt)
   {
     this.fromData(idx);

     var finished = false;
     if (this.pos[1] > 2.0) 
     {
        var y = -1.5 - this.radius - getRandom(0.05, 0.1); // y + radius
        this.pos[1] = y; // put this sphere behind the sphere it's following
        //console.log("reset: "+y +" "+previ
     }
     this.pos[0] = this.vel[0] + Math.sin(theta+idx)*0.15; // hack, vel[0] has starting x
     this.pos[1] += this.vel[1] * dt;
     this.pos[2] += this.vel[2] * dt;
     this.toData(idx);

     return finished;
   }
}

function initGL(canvas)
{
   try
   {
      gl = canvas.getContext("experimental-webgl");
      gl.viewportWidth = canvas.width;
      gl.viewportHeight = canvas.height;
      gl.getExtension('OES_texture_float'); 
   }
   catch (e)
  {
   }
   if (!gl)
   {
      alert("No WebGL for you");
   }
}

function getShader(gl, src, type)
{
   var shader = gl.createShader(type);
   gl.shaderSource(shader, src);
   gl.compileShader(shader);

   if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS))
   {
      alert(gl.getShaderInfoLog(shader));
      return null;
   }

   return shader;
}

function initShader(fragmentShader, vertexShader)
{
   var sp = gl.createProgram();
   gl.attachShader(sp, vertexShader);
   gl.attachShader(sp, fragmentShader);
   gl.linkProgram(sp);

   if (!gl.getProgramParameter(sp, gl.LINK_STATUS))
   {
      alert("Could not initialise shaders");
   }

   sp.vertexPositionAttribute = gl.getAttribLocation(sp, "pos");
   gl.enableVertexAttribArray(sp.vertexPositionAttribute);

   return sp;
}

function initShaders()
{
   var fragmentBackground = getShader(gl, fragmentProgram, gl.FRAGMENT_SHADER);
   var vertexShader = getShader(gl, vertexProgram, gl.VERTEX_SHADER);
   shaderProgram = initShader(fragmentBackground, vertexShader);
}

function initTexture()
{
   var num_of_floats = MAX_SPHERES * 3 * 4;

   setupSpheres();

   textureId = gl.createTexture();
   gl.bindTexture(gl.TEXTURE_2D, textureId);
   gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);
   gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);
   gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
   gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);

   textureSize = MAX_SPHERES * 3;

   gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, textureSize, 1, 0, gl.RGBA, gl.FLOAT, data);
   gl.bindTexture(gl.TEXTURE_2D, null);
}

function createGlBuffer(values, itemSize, numItems, type)
{
   var buffer = gl.createBuffer();
   gl.bindBuffer(gl.ARRAY_BUFFER, buffer);
   gl.bufferData(gl.ARRAY_BUFFER, values, type);
   buffer.itemSize = itemSize;
   buffer.numItems = numItems;
   return buffer;
}

function getSquareVertices (vertices)
{
  // GLfloat camera_positon = {0, 0, 2};
  var near_plane = 2;
  var fov = 30;
  var screen = [720, 480];

  var world_height;
  var world_width;

  world_height = 2 * Math.tan(fov);
  world_width = (world_height * screen[0]) / screen[1];

  var p1x = -(world_width/2);
  var p1y = world_height/2;
  var p1z = 0;

  var p2x = world_width/2;
  var p2y = world_height/2;
  var p2z = 0;

  var p3x = world_width/2;
  var p3y = -(world_height/2);
  var p3z = 0;

  var p4x = -(world_width/2);
  var p4y = -(world_height/2);
  var p4z = 0;

  vertices[0] = p1x;
  vertices[1] = p1y;
  vertices[2] = p1z;

  vertices[3] = p2x;
  vertices[4] = p2y;
  vertices[5] = p2z;

  vertices[6] = p3x;
  vertices[7] = p3y;
  vertices[8] = p3z;

  vertices[9] = p4x;
  vertices[10] = p4y;
  vertices[11] = p4z;
}

function initBuffers()
{
   getSquareVertices(vertices);
   mat4.ortho(pMatrix, -1.0, 1.0, -1.0, 1.0, -1.0, 10.0);

   var flatVerticesBuffer = [];
   for (var i = 0; i < vertices.length; i+=3) {
      x = vertices[i+0]
      y = vertices[i+1]
      z = vertices[i+2]
      var tmpVec4 = vec4.create()
      vec4.set(tmpVec4, x, y, z, 1)

      vec4.transformMat4(tmpVec4, tmpVec4, pMatrix)
      flatVerticesBuffer.push(tmpVec4[0], tmpVec4[1], tmpVec4[2]);
   }

   squareVertexPositionBuffer = createGlBuffer(new Float32Array(flatVerticesBuffer), 3, 4, gl.STATIC_DRAW);
}

function drawScene()
{
   gl.viewport(0, 0, gl.viewportWidth, gl.viewportHeight);
   gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

   gl.useProgram(shaderProgram);
   gl.bindBuffer(gl.ARRAY_BUFFER, squareVertexPositionBuffer);
   gl.vertexAttribPointer(shaderProgram.vertexPositionAttribute, squareVertexPositionBuffer.itemSize, gl.FLOAT, false, 0, 0);

   var utheta = gl.getUniformLocation(shaderProgram, "theta");
   gl.uniform1f(utheta, theta);

   var uTextureSize = gl.getUniformLocation(shaderProgram, "size_of_texture");
   gl.uniform1f(uTextureSize, textureSize);

   var uNumSpheres = gl.getUniformLocation(shaderProgram, "num_of_spheres");
   gl.uniform1f(uNumSpheres, numSpheres);

   gl.activeTexture(gl.TEXTURE0);
   gl.bindTexture(gl.TEXTURE_2D, textureId);
   gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, textureSize, 1, 0, gl.RGBA, gl.FLOAT, data);

   var uSphereInfo = gl.getUniformLocation(shaderProgram, "sphere_info");
   gl.uniform1i(uSphereInfo, 0);

   gl.drawArrays(gl.TRIANGLE_FAN, 0, squareVertexPositionBuffer.numItems);
}


function tick()
{
   var newTime = new Date().getTime();
   var dt = (newTime - lastTime)*0.001;
   theta += dt * 4.0;
    
   sh = new SphereHelper();
   for (var i = 0; i < numSpheres; ++i) 
   {
      sh.update(i, dt);
   }

   requestAnimFrame(tick);
   drawScene();
   lastTime = newTime
}

function getRandom(min, max)
{
   return Math.random() * (max - min) + min;
}

function setupSpheres()
{
   data = new Float32Array(MAX_SPHERES * 3 * 4)
   for (var i = 0; i < data.length; i++)
   {
      data[i] = 0.0;
   }

   var prevR = 0.0;
   var averageX = -2.0;
   var posY = -3.0;

   sh = new SphereHelper();
   sh.vel = [0.0, 1.0, 0.0];
   sh.pos = [0.0, 0.0, -2.0];
   sh.rgb = [0.5, 0.0, 0.0];
   sh.radius = 0.25;

   for (var i = 0; i < numSpheres; ++i) 
   {
      sh.radius = getRandom(0.1, 0.3);
      posY -= (prevR + sh.radius + getRandom(0.05, 0.1)); 
      sh.vel[0] = getRandom(0, 0.1) + averageX; 
      sh.vel[1] = getRandom(0.8, 1.5);
      sh.pos[1] = posY;
      sh.pos[2] = -2.0;
      sh.rgb[0] = getRandom(0, 1);
      sh.rgb[1] = getRandom(0, 1);
      sh.rgb[2] = getRandom(0, 1);
      prevR = sh.radius;
      sh.toData(i);

      if (i > 0 && i % 10 == 0)
      {
         averageX += 1.0 + getRandom(-0.1, 0.1);
         prevR = 0.0;
         posY = -3.0;
         console.log(averageX);
      }
   }
}

function webGLStart()
{
   var canvas = document.getElementById("canvas");
   initGL(canvas);
   initShaders();
   initBuffers();
   initTexture();

   lastTime =  new Date().getTime();
   gl.clearColor(0.1, 0.1, 0.1, 1.0);
   tick();
}


