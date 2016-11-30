var squareVertexPositionBuffer;
var mvMatrix = mat4.create();
var pMatrix = mat4.create();
var shaderProgram;
var gl;
var vertices = [];

function initGL(canvas)
{
   try
   {
      gl = canvas.getContext("experimental-webgl");
      gl.viewportWidth = canvas.width;
      gl.viewportHeight = canvas.height;
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
  var theta = 30;
  var screen = [720, 480];

  var world_height;
  var world_width;

  world_height = 2 * Math.tan(theta);
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
   var newVec3 = vec3.create();
   mat4.perspective(pMatrix, 45, gl.viewportWidth / gl.viewportHeight, 0.1, 100.0);
   mat4.identity(mvMatrix);
   var translationVec3 = vec3.set(newVec3, 0, 0, -10);
   mat4.translate(mvMatrix, mvMatrix, translationVec3);

   var newVec01 = vec4.create();
   var newVec02 = vec4.create();
   var newVec03 = vec4.create();
   var newVec04 = vec4.create();

   getSquareVertices(vertices);
   // var v1 = vec4.set(newVec01, 1.0,  1.0,  0.0, 1.0);
   // var v2 = vec4.set(newVec02, -1.0,  1.0,  0.0, 1.0);
   // var v3 = vec4.set(newVec03, 1.0,  -1.0,  0.0, 1.0);
   // var v4 = vec4.set(newVec04, -1.0,  -1.0,  0.0, 1.0);
   // vertices = [ v1, v2, v3, v4 ];

   var flatVerticesBuffer = [];

   // //mat4.ortho = function (out, left, right, bottom, top, near, far) {
   // mat4.ortho(pMatrix, -1.0, 1.0, -1.0, 1.0, -10.0, 10.0);

   for (var i = 0; i < vertices.length; i+=3) {
      console.log('before vertices[i] : ', vertices[i])
      console.log('B pMatrix : ', pMatrix)

      x = vertices[i+0]
      y = vertices[i+1]
      z = vertices[i+2]
      var tmpVec4 = vec4.create()
      vec4.set(tmpVec4, x, y, z)


      // vec4.transformMat4(vertices[i], vertices[i], mvMatrix);
      vec4.transformMat4(tmpVec4, tmpVec4, pMatrix)
      // vec4.transformMat4(vertices[i], vertices[i], pMatrix);
      console.log('A pMatrix : ', pMatrix)
      flatVerticesBuffer.push(tmpVec4[0]/tmpVec4[3], tmpVec4[1]/tmpVec4[3], tmpVec4[2]/tmpVec4[3]);
      console.log('after vertices[i] : ', vertices[i])
   }

   squareVertexPositionBuffer = createGlBuffer(new Float32Array(flatVerticesBuffer), 3, 4, gl.STATIC_DRAW);
}

function drawScene()
{
   gl.viewport(0, 0, gl.viewportWidth, gl.viewportHeight);
   gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

   // mat4.perspective(45, gl.viewportWidth / gl.viewportHeight, 0.1, 100.0, pMatrix);
   // mat4.identity(mvMatrix);
   // mat4.translate(mvMatrix, [0,0,-10]);

   gl.useProgram(shaderProgram);
   gl.bindBuffer(gl.ARRAY_BUFFER, squareVertexPositionBuffer);
   gl.vertexAttribPointer(shaderProgram.vertexPositionAttribute, squareVertexPositionBuffer.itemSize, gl.FLOAT, false, 0, 0);

   gl.drawArrays(gl.TRIANGLE_STRIP, 0, squareVertexPositionBuffer.numItems);
}

function webGLStart()
{
   var canvas = document.getElementById("canvas");
   initGL(canvas);
   initShaders();
   initBuffers();

   gl.clearColor(0.1, 0.1, 0.1, 1.0);
   drawScene();
}


