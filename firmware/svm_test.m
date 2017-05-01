theta = 0:pi/100:4*pi;
duty = zeros(length(theta),3);

Vbus = 13;
half_Vbus = Vbus/2.0;
% q-axis voltage and d-axis voltage
Vq = 1;
Vd = 0.0;
%normalize voltages
uq = Vq/half_Vbus;
ud = Vd/half_Vbus;

% % q-axis and d-axis voltages
% % gives duty cycles from 0 to 1;
% uq = 0.6123724;    % sqrt(3/2)/2
% ud = 0.0;
% 
% % uq^2 + ud^2 = (sqrt(3/2)/2)^2 also gives duty cycles from 0 to 1
% uq = 0.5590170;  
% ud = 0.25;
% 
% % much to large
% uq = 1;  
% ud = 1;

maxduty = 0.85;
% max_mag = sqrt(3/2)(maxduty-0.5)
max_mag = 1.224745*(maxduty - 0.5);
mag = sqrt(uq^2 + ud^2);
if ( mag > max_mag)
    scale = max_mag/mag;
    uq = uq*scale;
    ud = ud*scale;
end
    
for i=1:length(theta)
    sth = sin(theta(i));
    cth = cos(theta(i));
    
    %alpha-beta duty thru iPark Transform
    ua = cth*ud - sth*uq;
    ub = sth*ud + cth*uq;
    
    % abc DutyCycles thu iClark Transform
    duty(i,1) = 0.8164966*ua + 0.5;
    duty(i,2) = -0.4082483*ua + 0.7071068*ub + 0.5;
    duty(i,3) = -0.4082483*ua - 0.7071068*ub + 0.5;
end

plot(theta,duty(:,1),theta,duty(:,2),theta,duty(:,3))