n = 50;
[X,Y,Z] = sphere(n);
offsetX = X;
offsetY = Y;
offsetZ = Z;

figure
s = surfl(offsetX, offsetY, offsetZ);
for T=1:0.1:20
for i=1:n+1
    for j=1:n+1
        offset = (1+cos(T))*0.25*3.14;
        scale = sin(X(i,j)*Z(i,j)*20.0+offset) * 0.5;
        normal = [X(i,j) Y(i,j) Z(i,j)]';
        normal = normal ./ norm(normal,2);
        
        offsetX(i,j) = scale*normal(1) + X(i,j);
        offsetY(i,j) = scale*normal(2) + Y(i,j);
        offsetZ(i,j) = scale*normal(3) + Z(i,j);
        
    end
end

s.XData = offsetX;
s.YData = offsetY;
s.ZData = offsetZ;
s = surfl(offsetX, offsetY, offsetZ);

refresh;
pause(0.1);
theta

end

syms theta phi r A B C

dx_dtheta = diff(cos(theta)*sin(phi), theta)
dy_dtheta = diff(cos(phi), theta)
dz_dtheta = diff(sin(theta)*sin(phi), theta)

dx_dphi = diff(cos(theta)*sin(phi), phi)
dy_dphi = diff(cos(phi), phi)
dz_dphi = diff(sin(theta)*sin(phi), phi)

normal_x = dy_dtheta * dz_dphi - dz_dtheta * dy_dphi;
normal_y = dz_dtheta * dx_dphi - dx_dtheta * dz_dphi;
normal_z = dx_dtheta * dy_dphi - dy_dtheta * dx_dphi;

% the normal is the same as x,y,z

val = sin(cos(theta)*sin(theta)*sin(phi)*sin(phi)*A + B) * C;
dx_dtheta = diff(cos(theta)*sin(phi)*(1+val), theta)
dy_dtheta = diff(cos(phi)*(1 + val), theta)
dz_dtheta = diff(sin(theta)*sin(phi)*(1 + val), theta)

dx_dphi = diff(cos(theta)*sin(phi)*(1 + val), phi)
dy_dphi = diff(cos(phi)*(1 + val), phi)
dz_dphi = diff(sin(theta)*sin(phi)*(1 + val), phi)


normal_x = dy_dtheta * dz_dphi - dz_dtheta * dy_dphi
normal_y = dz_dtheta * dx_dphi - dx_dtheta * dz_dphi
normal_z = dx_dtheta * dy_dphi - dy_dtheta * dx_dphi

% answer
normal_x =
(sin(phi)*(C*sin(A*cos(theta)*sin(theta)*sin(phi)^2 + B) + 1) - 2*A*C*cos(A*cos(theta)*sin(theta)*sin(phi)^2 + B)*cos(phi)^2*cos(theta)*sin(phi)*sin(theta))*(cos(theta)*sin(phi)*(C*sin(A*cos(theta)*sin(theta)*sin(phi)^2 + B) + 1) + C*cos(A*cos(theta)*sin(theta)*sin(phi)^2 + B)*sin(phi)*sin(theta)*(A*cos(theta)^2*sin(phi)^2 - A*sin(phi)^2*sin(theta)^2)) + C*cos(A*cos(theta)*sin(theta)*sin(phi)^2 + B)*cos(phi)*(A*cos(theta)^2*sin(phi)^2 - A*sin(phi)^2*sin(theta)^2)*(2*A*C*cos(A*cos(theta)*sin(theta)*sin(phi)^2 + B)*cos(phi)*cos(theta)*sin(phi)^2*sin(theta)^2 + cos(phi)*(C*sin(A*cos(theta)*sin(theta)*sin(phi)^2 + B) + 1)*sin(theta))
 
 
normal_y =
(2*A*C*cos(A*cos(theta)*sin(theta)*sin(phi)^2 + B)*cos(phi)*sin(theta)*cos(theta)^2*sin(phi)^2 + cos(phi)*(C*sin(A*cos(theta)*sin(theta)*sin(phi)^2 + B) + 1)*cos(theta))*(cos(theta)*sin(phi)*(C*sin(A*cos(theta)*sin(theta)*sin(phi)^2 + B) + 1) + C*cos(A*cos(theta)*sin(theta)*sin(phi)^2 + B)*sin(phi)*sin(theta)*(A*cos(theta)^2*sin(phi)^2 - A*sin(phi)^2*sin(theta)^2)) + (sin(phi)*sin(theta)*(C*sin(A*cos(theta)*sin(theta)*sin(phi)^2 + B) + 1) - C*cos(A*cos(theta)*sin(theta)*sin(phi)^2 + B)*cos(theta)*sin(phi)*(A*cos(theta)^2*sin(phi)^2 - A*sin(phi)^2*sin(theta)^2))*(2*A*C*cos(A*cos(theta)*sin(theta)*sin(phi)^2 + B)*cos(phi)*cos(theta)*sin(phi)^2*sin(theta)^2 + cos(phi)*(C*sin(A*cos(theta)*sin(theta)*sin(phi)^2 + B) + 1)*sin(theta))
 
 
normal_z =
(sin(phi)*(C*sin(A*cos(theta)*sin(theta)*sin(phi)^2 + B) + 1) - 2*A*C*cos(A*cos(theta)*sin(theta)*sin(phi)^2 + B)*cos(phi)^2*cos(theta)*sin(phi)*sin(theta))*(sin(phi)*sin(theta)*(C*sin(A*cos(theta)*sin(theta)*sin(phi)^2 + B) + 1) - C*cos(A*cos(theta)*sin(theta)*sin(phi)^2 + B)*cos(theta)*sin(phi)*(A*cos(theta)^2*sin(phi)^2 - A*sin(phi)^2*sin(theta)^2)) - C*cos(A*cos(theta)*sin(theta)*sin(phi)^2 + B)*cos(phi)*(A*cos(theta)^2*sin(phi)^2 - A*sin(phi)^2*sin(theta)^2)*(2*A*C*cos(A*cos(theta)*sin(theta)*sin(phi)^2 + B)*cos(phi)*sin(theta)*cos(theta)^2*sin(phi)^2 + cos(phi)*(C*sin(A*cos(theta)*sin(theta)*sin(phi)^2 + B) + 1)*cos(theta))
 

