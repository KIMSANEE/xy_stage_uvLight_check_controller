clear all; clc; 
delete(instrfindall);
s=serial('COM7');
set(s, 'BaudRate', 115200);
s.InputBufferSize = 100;

bg = 260; % background data

number = 300;
standard = 120;
valocity = 1.0;
distance = 5;
check = 1;

Center_X = 136;
Center_Y = 148;

fopen(s);

% --------Input Data to Arduino------------ %
fprintf(s,'%d\n',number);
fprintf(s, '%d\n', standard);
fprintf(s, '%f\n',valocity);
fprintf(s,' %d\n',distance);
fprintf(s,'%d\n',check);
% ----------------------------------------- %

fprintf(s,"Initialize");

fprintf(s,'%d\n',Center_X);
fprintf(s,'%d\n',Center_Y);

fprintf(s,"Go");

B= zeros(standard/distance+1,standard/distance+1);

for i = 1: standard/distance+1
    for j = 1: standard/distance+1
        
        % close(gcf);
        [X,Y]=meshgrid(0:standard/distance, 0:standard/distance);
        Z=B(1:standard/distance+1,1:standard/distance+1);
        figure(1); surf(X,Y,Z);
        xlabel('X(mm)');
        ylabel('Y(mm)');
        zlabel('Z(lux)');
        view(2);
        fprintf(s,'DataRead');
        pause(0.001*number);
        out = fscanf(s);
        if j~= standard/distance+1
            if mod(i,2) ~= 0
                B(i,j) = str2double(out)-bg;
                fprintf(s,'moveRightX');
                % pause(0.2);
            else
                 B(i,standard/distance+2-j)=str2double(out)-bg;
                 fprintf(s,'moveLeftX');
                 % pause(0.2);
            end
        elseif i~= standard/distance+1
            if mod(i,2)~=0
                B(i,j)=str2double(out)-bg;
            else
                B(i,standard/distance+2-j)=str2double(out)-bg;
            end 
        fprintf(s,'moveRightY');
        else
            if mod(i,2)~=0
            B(i,j)=str2double(out)-bg;
            else
            B(i,standard/distance+2-j)=str2double(out)-bg;
            end
        end
        drawnow; % 그림 업데이트
    end
end

fprintf(s,"done");

%  --------------------plot graph----------------------------------
[X,Y]=meshgrid(0:standard/distance, 0:standard/distance);
Z=B(1:standard/distance+1,1:standard/distance+1);
figure; surf(X,Y,Z);
xlabel('X(mm)');
ylabel('Y(mm)');
zlabel('Z(lx)');
% -----------------------------------------------------------------

fclose(s);

