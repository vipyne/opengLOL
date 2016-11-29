var squareVertexPositionBuffer;
var mvMatrix = mat4.create();
var pMatrix = mat4.create();
var shaderProgram;
var gl;

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

   sp.vertexPositionAttribute = gl.getAttribLocation(sp, "aVertexPosition");
   gl.enableVertexAttribArray(sp.vertexPositionAttribute);

   sp.pMatrixUniform = gl.getUniformLocation(sp, "uPMatrix");
   sp.mvMatrixUniform = gl.getUniformLocation(sp, "uMVMatrix");

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

function initBuffers() 
{
   var vertices = [
       1.0,  1.0,  0.0,
      -1.0,  1.0,  0.0,
       1.0, -1.0,  0.0,
      -1.0, -1.0,  0.0
   ];
   squareVertexPositionBuffer = createGlBuffer(new Float32Array(vertices), 3, 4, gl.STATIC_DRAW);
}

function drawScene() 
{
   gl.viewport(0, 0, gl.viewportWidth, gl.viewportHeight);
   gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

   mat4.perspective(45, gl.viewportWidth / gl.viewportHeight, 0.1, 100.0, pMatrix);
   mat4.identity(mvMatrix);
   mat4.translate(mvMatrix, [0,0,-10]);
 
   gl.useProgram(shaderProgram);
   gl.bindBuffer(gl.ARRAY_BUFFER, squareVertexPositionBuffer);
   gl.vertexAttribPointer(shaderProgram.vertexPositionAttribute, squareVertexPositionBuffer.itemSize, gl.FLOAT, false, 0, 0);
 
   gl.uniformMatrix4fv(shaderProgram.pMatrixUniform, false, pMatrix);
   gl.uniformMatrix4fv(shaderProgram.mvMatrixUniform, false, mvMatrix);

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


