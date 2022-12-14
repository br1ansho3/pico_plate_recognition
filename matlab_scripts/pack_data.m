clear
clc
format compact

load("ex4data1.mat");
load("ex4weights.mat");
X(abs(X)>=0.5)=1;
X(abs(X)<0.5)=0;

%{
% Pack ex4data1
fid = fopen('ex4data1_binary','w');
data = num2str(X,'%.0f,');

for i = 1:5000
    fprintf(fid, [(data(i,:))]);
end

fclose(fid);
%}



% Pack ex4data1
fid1 = fopen('ex4data1_binary_train_4000','w');
data = num2str(X,'%.0f,');
for j = 0:9
    for i = 1:400
        fprintf(fid1, [(data(i+(j*500),:))]);
    end
end


fclose(fid1);



%{
% Pack label
f_label = fopen('label_ttt','w');
label = num2str(y);
for i = 1:5000
     fprintf(f_label, [(label(i,:)),'\n']);
end
fclose(f_label);
%}


% Pack label
f_label1 = fopen('label_binary_train_4000','w');
label = num2str(y);
for j = 0:9
    for i = 1:400
         fprintf(f_label1, [(label(i+(500*j),:)) ',']);
    end
end

fclose(f_label1);


%{
% Pack ex4weight_theta1
f_weight1 = fopen('ex4weight_theta1','w');
weight1 = num2str(Theta1,'%.8f,');
for i = 1:25
     fprintf(f_weight1, [(weight1(i,:))]);
end
fclose(f_weight1);

% Pack ex4weight_theta2
f_weight2 = fopen('ex4weight_theta2','w');
weight2 = num2str(Theta2,'%.8f,');
for i = 1:10
     fprintf(f_weight2, [(weight2(i,:))]);
end
fclose(f_weight2);
%}

disp("done");
