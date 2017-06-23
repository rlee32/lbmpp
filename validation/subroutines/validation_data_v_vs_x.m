function [x, v] = validation_data_v_vs_x(Re)
% Returns the Ghia et al. validation data for lid driven cavity at Re =
% 100.
% centerline v-velocity vs. x.

x = 0;
v = 0;

if Re == 100
x = [1.0, 0.9688, 0.9609, 0.9531, 0.9453, 0.9063, 0.8594, 0.8047, ...
    0.50, 0.2344, 0.2266, 0.1563, 0.0938, 0.0781, 0.0703, 0.0625, 0.0];
v = [ 0.0, -0.05906, -0.07391, -0.08864, -0.10313, -0.16914, -0.22445, ...
    -0.24533, 0.05454, 0.17527, 0.17507, 0.16077, 0.12317, 0.10890, 0.10091, ...
    0.09233, 0.0];
end
if Re == 1000
x = [1.0, 0.9688, 0.9609, 0.9531, 0.9453, 0.9063, 0.8594, 0.8047, ...
    0.50, 0.2344, 0.2266, 0.1563, 0.0938, 0.0781, 0.0703, 0.0625, 0.0];
v = [ 0.0, -0.21388, -0.27669, -0.33714, -0.39188, -0.51550, -0.42665, ...
    -0.31966, 0.02526, 0.32235, 0.33075, 0.37095, 0.32627, 0.30353, ...
    0.29012, 0.27485, 0.0];
end
if Re == 5000
x = [1.0, 0.9688, 0.9609, 0.9531, 0.9453, 0.9063, 0.8594, 0.8047, ...
    0.50, 0.2344, 0.2266, 0.1563, 0.0938, 0.0781, 0.0703, 0.0625, 0.0];
v = [ 0.0, -0.49774, -0.55069, -0.55408, -0.52876, -0.41442, -0.36214, ...
    -0.30018, 0.00945, 0.27280, 0.28066, 0.35368, 0.42951, 0.43648, 0.43329, ...
    0.42447, 0.0];
end
if Re == 10000
x = [1.0, 0.9688, 0.9609, 0.9531, 0.9453, 0.9063, 0.8594, 0.8047, ...
    0.50, 0.2344, 0.2266, 0.1563, 0.0938, 0.0781, 0.0703, 0.0625, 0.0];
v = [ 0.0, -0.54302, -0.52987, -0.49099, -0.45863, -0.41496, -0.36737, ...
    -0.30719, 0.00831, 0.27224, 0.28003, 0.35070, 0.41487, 0.43124, 0.43733, ...
    0.43983, 0.0];
end
if Re == 21000
x = [1.0:-0.015:0.85, 0.5, 0.15:-0.015:0.0];
v = [ 0.0000, -0.4522, -0.5266, -0.4588, -0.4449, -0.4287, -0.4110, ...
    -0.3936, -0.3764, -0.3593, -0.3425, 0.0063, 0.3413, 0.3567, 0.3725, ...
    0.3885, 0.4048, 0.4218, 0.4420, 0.4596, 0.4357, 0.3323, 0.0];
end