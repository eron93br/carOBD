mair = 28.9; % molecular weight of air
vd = 1.5;    % engine displacement volume
nvol = 0.85;  % engine volumetric effiency multiplier
R = 8.314; % universal gas constant
CR = 10.5; % engine compression ratio

const = (map - (barometric/CR))/(R*iat);
maf = mair*vd*nvol*const;+

norm_r = (rpm - min(rpm)) / (max(rpm) - min(rpm) );
