//Maya ASCII 2022 scene
//Name: sheep.ma
//Last modified: Sat, May 20, 2023 12:13:19 AM
//Codeset: 1252
requires maya "2022";
requires "mtoa" "5.0.0.1";
currentUnit -l centimeter -a degree -t film;
fileInfo "application" "maya";
fileInfo "product" "Maya 2022";
fileInfo "version" "2022";
fileInfo "cutIdentifier" "202110272215-ad32f8f1e6";
fileInfo "osv" "Windows 10 Home v2009 (Build: 22621)";
fileInfo "UUID" "961634F7-4AA1-44A5-DFCB-71828F5E1911";
createNode transform -s -n "persp";
	rename -uid "7DB44293-4650-65DA-AE64-EBB59DAB207C";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 64.201123589089747 57.823771356519948 77.968232005886762 ;
	setAttr ".r" -type "double3" -17.738352729901369 38.199999999999818 0 ;
createNode camera -s -n "perspShape" -p "persp";
	rename -uid "7DA236A6-4082-2EB8-49E3-34A76896B9C1";
	setAttr -k off ".v" no;
	setAttr ".fl" 34.999999999999993;
	setAttr ".coi" 112.70794290811278;
	setAttr ".imn" -type "string" "persp";
	setAttr ".den" -type "string" "persp_depth";
	setAttr ".man" -type "string" "persp_mask";
	setAttr ".hc" -type "string" "viewSet -p %camera";
createNode transform -s -n "top";
	rename -uid "116EEA76-46F0-1CA7-F7F9-BE81DDF726FA";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 0 1000.1 0 ;
	setAttr ".r" -type "double3" -90 0 0 ;
createNode camera -s -n "topShape" -p "top";
	rename -uid "9199B541-486A-9257-0882-F0B14B9F84C4";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 1000.1;
	setAttr ".ow" 30;
	setAttr ".imn" -type "string" "top";
	setAttr ".den" -type "string" "top_depth";
	setAttr ".man" -type "string" "top_mask";
	setAttr ".hc" -type "string" "viewSet -t %camera";
	setAttr ".o" yes;
	setAttr ".ai_translator" -type "string" "orthographic";
createNode transform -s -n "front";
	rename -uid "C0AB5F46-4A4C-169B-8686-B8B70AB91AD0";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 0 0 1000.1 ;
createNode camera -s -n "frontShape" -p "front";
	rename -uid "6B900027-4C5D-15EE-F437-CA90C9F5FAF4";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 1000.1;
	setAttr ".ow" 30;
	setAttr ".imn" -type "string" "front";
	setAttr ".den" -type "string" "front_depth";
	setAttr ".man" -type "string" "front_mask";
	setAttr ".hc" -type "string" "viewSet -f %camera";
	setAttr ".o" yes;
	setAttr ".ai_translator" -type "string" "orthographic";
createNode transform -s -n "side";
	rename -uid "759AA787-42A0-7115-A728-6A9DA4361EDB";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 1000.1 0 0 ;
	setAttr ".r" -type "double3" 0 90 0 ;
createNode camera -s -n "sideShape" -p "side";
	rename -uid "69AA11E6-40DC-5647-5DDD-56877BD0AD05";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 1000.1;
	setAttr ".ow" 30;
	setAttr ".imn" -type "string" "side";
	setAttr ".den" -type "string" "side_depth";
	setAttr ".man" -type "string" "side_mask";
	setAttr ".hc" -type "string" "viewSet -s %camera";
	setAttr ".o" yes;
	setAttr ".ai_translator" -type "string" "orthographic";
createNode transform -n "Sheep1";
	rename -uid "0F47071B-4761-6DF2-623D-7BA05A218D20";
	setAttr ".t" -type "double3" 0 0 -4 ;
	setAttr ".rp" -type "double3" -3.412115212597655e-08 28.380465822041604 4.5132998793002663 ;
	setAttr ".sp" -type "double3" -3.412115212597655e-08 28.380465822041604 4.5132998793002663 ;
createNode mesh -n "Sheep1Shape" -p "Sheep1";
	rename -uid "7D2C3B7A-4817-BD4A-F598-7BAC9CC83455";
	setAttr -k off ".v";
	setAttr -s 2 ".iog[0].og";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "UVChannel_1";
	setAttr -s 288 ".uvst[0].uvsp";
	setAttr ".uvst[0].uvsp[0:249]" -type "float2" 0.8125 0.875 0.8125 0.7814371
		 0.53120363 0.78133202 0.53120363 0.875 0.67162764 0.875 0.875 0.53168947 0.875 0.7812857
		 0.7502526 0.7812857 0.75 0.53168947 0.67172021 0.7814371 0.67162764 0.875 0.75 0.53168947
		 0.65610993 0.53168947 0.53095102 0.53173578 0.65610993 0.53168947 0.67172021 0.7814371
		 0.4375 0.53168947 0.53095102 0.53173578 0.65636253 0.7812857 0.53120363 0.78133202
		 0.53120363 0.78133202 0.4375 0.7812857 0.7502526 0.7812857 0.65636253 0.7812857 0.43747863
		 0.875 0.21876 1 0.125 0.78142577 0.125 0.875 0.34375998 0.875 0.125 0.78142577 0.125
		 0.875 0.21876001 0.875 0.125 0.875 0.21876 0.78142577 0.21876 0.875 0 0.875 0 0.78142577
		 0.34375998 0.875 0.34375998 0.78142577 0.31257579 1 0.125 1 0.21876 1 0.31269073
		 0.875 0.21876001 0.875 0.43747863 0.78142577 0.34375998 0.78142577 0.21876001 0.875
		 0.21876001 0.78142577 0.0625 0.75 0.125 0.75 0.0625 0.5 0.0625 0.6875 0.125 0.75
		 0.0625 0.5 0.0625 0.6875 0.125 0.6875 0.0625 0.6875 0.125 0.5 0.125 0.6875 0 0.6875
		 0 0.5 0.1875 0.6875 0.1875 0.5 0.1875 0.75 0.25 0.6875 0.25 0.5 0.1875 0.6875 0.125
		 0.6875 0.1875 0.6875 0.1875 0.5 0.125 0.6875 0.125 0.5 0.0625 0.75 0.125 0.75 0.0625
		 0.5 0.0625 0.6875 0.125 0.75 0.0625 0.5 0.0625 0.6875 0.125 0.6875 0.0625 0.6875
		 0.125 0.5 0.125 0.6875 0 0.6875 0 0.5 0.1875 0.6875 0.1875 0.5 0.1875 0.75 0.25 0.6875
		 0.25 0.5 0.1875 0.6875 0.125 0.6875 0.1875 0.6875 0.1875 0.5 0.125 0.6875 0.125 0.5
		 0.0625 0.75 0.125 0.75 0.0625 0.5 0.0625 0.6875 0.125 0.75 0.0625 0.5 0.0625 0.6875
		 0.125 0.6875 0.0625 0.6875 0.125 0.5 0.125 0.6875 0 0.6875 0 0.5 0.1875 0.6875 0.1875
		 0.5 0.1875 0.75 0.25 0.6875 0.25 0.5 0.1875 0.6875 0.125 0.6875 0.1875 0.6875 0.1875
		 0.5 0.125 0.6875 0.125 0.5 0.0625 0.75 0.125 0.75 0.0625 0.5 0.0625 0.6875 0.125
		 0.75 0.0625 0.5 0.0625 0.6875 0.125 0.6875 0.0625 0.6875 0.125 0.5 0.125 0.6875 0
		 0.6875 0 0.5 0.1875 0.6875 0.1875 0.5 0.1875 0.75 0.25 0.6875 0.25 0.5 0.1875 0.6875
		 0.125 0.6875 0.1875 0.6875 0.1875 0.5 0.125 0.6875 0.125 0.5 0.8125 0.2814371 0.8125
		 0.375 0.67162764 0.375 0.67172021 0.2814371 0.53120363 0.28133202 0.67172021 0.2814371
		 0.67162764 0.375 0.53120363 0.375 0.53095102 0.031735778 0.65610993 0.031689465 0.65636253
		 0.2812857 0.53120363 0.28133202 0.65610993 0.031689465 0.75 0.031689465 0.7502526
		 0.2812857 0.65636253 0.2812857 0.75 0.031689465 0.875 0.031689465 0.875 0.2812857
		 0.7502526 0.2812857 0.4375 0.031689465 0.53095102 0.031735778 0.53120363 0.28133202
		 0.4375 0.2812857 0.37535623 0.31210202 0.37535623 0.40567625 0.28144684 0.40567625
		 0.28144684 0.31210202 0.093946852 0.31210202 0.18770686 0.31210202 0.18770686 0.40567625
		 0.093946852 0.40567625 0.18763107 0.5 0.28144684 0.5 0.28144684 0.40567625 0.18770686
		 0.40567625 0.28144684 0.31210202 0.28144684 0.40567625 0.18770686 0.40567625 0.18770686
		 0.31210202 0.18763107 0.5 0.093871064 0.5 0.093946852 0.40567625 0.18770686 0.40567625
		 0.00034150342 0.40567625 0.00034150342 0.31210202 0.093946852 0.31210202 0.093946852
		 0.40567625 0.25 0.081087053 0.25 0.1875 0.1875 0.1875 0.1875 0.081087053 0.0625 0.081087053
		 0.125 0.081087053 0.125 0.1875 0.0625 0.1875 0.125 0.25 0.1875 0.25 0.1875 0.1875
		 0.125 0.1875 0.1875 0.081087053 0.1875 0.1875 0.125 0.1875 0.125 0.081087053 0.125
		 0.25 0.0625 0.25 0.0625 0.1875 0.125 0.1875 0 0.1875 0 0.081087053 0.0625 0.081087053
		 0.0625 0.1875 0.25 0.081087053 0.25 0.1875 0.1875 0.1875 0.1875 0.081087053 0.0625
		 0.081087053 0.125 0.081087053 0.125 0.1875 0.0625 0.1875 0.125 0.25 0.1875 0.25 0.1875
		 0.1875 0.125 0.1875 0.1875 0.081087053 0.1875 0.1875 0.125 0.1875 0.125 0.081087053
		 0.125 0.25 0.0625 0.25 0.0625 0.1875 0.125 0.1875 0 0.1875 0 0.081087053 0.0625 0.081087053
		 0.0625 0.1875 0.25 0.081087053 0.25 0.1875 0.1875 0.1875 0.1875 0.081087053 0.0625
		 0.081087053 0.125 0.081087053 0.125 0.1875 0.0625 0.1875 0.125 0.25 0.1875 0.25;
	setAttr ".uvst[0].uvsp[250:287]" 0.1875 0.1875 0.125 0.1875 0.1875 0.081087053
		 0.1875 0.1875 0.125 0.1875 0.125 0.081087053 0.125 0.25 0.0625 0.25 0.0625 0.1875
		 0.125 0.1875 0 0.1875 0 0.081087053 0.0625 0.081087053 0.0625 0.1875 0.25 0.081087053
		 0.25 0.1875 0.1875 0.1875 0.1875 0.081087053 0.0625 0.081087053 0.125 0.081087053
		 0.125 0.1875 0.0625 0.1875 0.125 0.25 0.1875 0.25 0.1875 0.1875 0.125 0.1875 0.1875
		 0.081087053 0.1875 0.1875 0.125 0.1875 0.125 0.081087053 0.125 0.25 0.0625 0.25 0.0625
		 0.1875 0.125 0.1875 0 0.1875 0 0.081087053 0.0625 0.081087053 0.0625 0.1875;
	setAttr ".cuvs" -type "string" "UVChannel_1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 96 ".vt[0:95]"  -10.15999985 30.53946877 -21.33926773 10.15999889 30.53946877 -21.33926773
		 -10.15999985 45.77946472 -21.33926392 10.15999889 45.77946472 -21.33926392 -10.15999985 30.53946114 19.30073166
		 10.15999889 30.53946114 19.30073166 -10.15999985 45.77946091 19.30073357 10.15999889 45.77946091 19.30073357
		 -7.62000036 40.69946671 14.22073269 7.61999941 40.69946671 14.22073269 -7.62000036 55.93946838 14.22073555
		 7.61999941 55.93946838 14.22073555 -7.62000036 40.69946289 34.54073334 7.61999941 40.69946289 34.54073334
		 -7.62000036 55.93946457 34.54073334 7.61999941 55.93946457 34.54073334 -12.69999981 0.059469223 6.6007309
		 -2.54000068 0.059469223 6.6007309 -12.69999981 30.53946877 6.60073614 -2.54000068 30.53946877 6.60073614
		 -12.69999981 0.059467316 16.76073074 -2.54000068 0.059467316 16.76073074 -12.69999981 30.53946686 16.76073456
		 -2.54000068 30.53946686 16.76073456 2.53999925 0.059469223 6.6007309 12.69999981 0.059469223 6.6007309
		 2.53999925 30.53946877 6.60073614 12.69999981 30.53946877 6.60073614 2.53999925 0.059467316 16.76073074
		 12.69999981 0.059467316 16.76073074 2.53999925 30.53946686 16.76073456 12.69999981 30.53946686 16.76073456
		 -12.69999981 0.059467316 -23.87926865 -2.53999925 0.059467316 -23.87926865 -12.69999981 30.53946877 -23.87926483
		 -2.53999925 30.53946877 -23.87926483 -12.69999981 0.059467316 -13.71926785 -2.53999925 0.059467316 -13.71926785
		 -12.69999981 30.53946686 -13.71926403 -2.53999925 30.53946686 -13.71926403 2.54000068 0.059467316 -23.87926865
		 12.69999981 0.059467316 -23.87926865 2.54000068 30.53946877 -23.87926483 12.69999981 30.53946877 -23.87926483
		 2.54000068 0.059467316 -13.71926785 12.69999981 0.059467316 -13.71926785 2.54000068 30.53946686 -13.71926403
		 12.69999981 30.53946686 -13.71926403 -15.23999977 26.7294693 -25.51413536 15.23999977 26.7294693 -25.51413536
		 -15.23999977 49.58946991 -25.51413155 15.23999977 49.58946991 -25.51413155 -15.23999977 26.72945976 23.47559547
		 15.23999977 26.72945976 23.47559547 -15.23999977 49.58945847 23.47559929 15.23999977 49.58945847 23.47559929
		 -8.38199997 39.93746567 14.088734627 8.38199997 39.93746567 14.088734627 -8.38199997 56.70146561 14.088737488
		 8.38199997 56.70146561 14.088737488 -8.38199997 39.93746185 30.8527317 8.38199997 39.93746185 30.8527317
		 -8.38199997 56.70146179 30.85273552 8.38199997 56.70146179 30.85273552 -13.97000027 15.29946136 5.3307333
		 -1.26999962 15.29946136 5.3307333 -13.97000027 34.3494606 5.33073902 -1.26999962 34.3494606 5.33073902
		 -13.97000027 15.29945946 18.030729294 -1.26999962 15.29945946 18.030729294 -13.97000027 34.34945679 18.030738831
		 -1.26999962 34.34945679 18.030738831 1.26999962 15.29946136 5.3307333 13.97000027 15.29946136 5.3307333
		 1.26999962 34.3494606 5.33073902 13.97000027 34.3494606 5.33073902 1.26999962 15.29945946 18.030729294
		 13.97000027 15.29945946 18.030729294 1.26999962 34.34945679 18.030738831 13.97000027 34.34945679 18.030738831
		 -13.96999931 15.29946518 -25.14926529 -1.26999879 15.29946518 -25.14926529 -13.96999931 34.34946442 -25.14925957
		 -1.26999879 34.34946442 -25.14925957 -13.96999931 15.29946327 -12.44926643 -1.26999879 15.29946327 -12.44926643
		 -13.96999931 34.34946442 -12.44925976 -1.26999879 34.34946442 -12.44925976 1.27000153 15.29946518 -25.14926529
		 13.97000217 15.29946518 -25.14926529 1.27000153 34.34946442 -25.14925957 13.97000217 34.34946442 -25.14925957
		 1.27000153 15.29946327 -12.44926643 13.97000217 15.29946327 -12.44926643 1.27000153 34.34946442 -12.44925976
		 13.97000217 34.34946442 -12.44925976;
	setAttr -s 144 ".ed[0:143]"  0 2 0 2 3 0 3 1 0 1 0 0 4 5 0 5 7 0 7 6 0
		 6 4 0 1 5 0 4 0 0 3 7 0 2 6 0 8 10 0 10 11 0 11 9 0 9 8 0 12 13 0 13 15 0 15 14 0
		 14 12 0 9 13 0 12 8 0 11 15 0 10 14 0 16 18 0 18 19 0 19 17 0 17 16 0 20 21 0 21 23 0
		 23 22 0 22 20 0 17 21 0 20 16 0 19 23 0 18 22 0 24 26 0 26 27 0 27 25 0 25 24 0 28 29 0
		 29 31 0 31 30 0 30 28 0 25 29 0 28 24 0 27 31 0 26 30 0 32 34 0 34 35 0 35 33 0 33 32 0
		 36 37 0 37 39 0 39 38 0 38 36 0 33 37 0 36 32 0 35 39 0 34 38 0 40 42 0 42 43 0 43 41 0
		 41 40 0 44 45 0 45 47 0 47 46 0 46 44 0 41 45 0 44 40 0 43 47 0 42 46 0 48 50 0 50 51 0
		 51 49 0 49 48 0 52 53 0 53 55 0 55 54 0 54 52 0 49 53 0 52 48 0 51 55 0 50 54 0 56 58 0
		 58 59 0 59 57 0 57 56 0 60 61 0 61 63 0 63 62 0 62 60 0 57 61 0 60 56 0 59 63 0 58 62 0
		 64 66 0 66 67 0 67 65 0 65 64 0 68 69 0 69 71 0 71 70 0 70 68 0 65 69 0 68 64 0 67 71 0
		 66 70 0 72 74 0 74 75 0 75 73 0 73 72 0 76 77 0 77 79 0 79 78 0 78 76 0 73 77 0 76 72 0
		 75 79 0 74 78 0 80 82 0 82 83 0 83 81 0 81 80 0 84 85 0 85 87 0 87 86 0 86 84 0 81 85 0
		 84 80 0 83 87 0 82 86 0 88 90 0 90 91 0 91 89 0 89 88 0 92 93 0 93 95 0 95 94 0 94 92 0
		 89 93 0 92 88 0 91 95 0 90 94 0;
	setAttr -s 288 ".n";
	setAttr ".n[0:165]" -type "float3"  0 6.4142e-08 -0.39370078 0 6.4142e-08
		 -0.39370078 0 6.4142e-08 -0.39370078 0 6.4142e-08 -0.39370078 0 -6.4142e-08 0.39370078
		 0 -6.4142e-08 0.39370078 0 -6.4142e-08 0.39370078 0 -6.4142e-08 0.39370078 0 -0.39370078
		 -6.4142e-08 0 -0.39370078 -6.4142e-08 0 -0.39370078 -6.4142e-08 0 -0.39370078 -6.4142e-08
		 0.39370078 0 0 0.39370078 0 0 0.39370078 0 0 0.39370078 0 0 0 0.39370078 6.4142e-08
		 0 0.39370078 6.4142e-08 0 0.39370078 6.4142e-08 0 0.39370078 6.4142e-08 -0.39370078
		 0 0 -0.39370078 0 0 -0.39370078 0 0 -0.39370078 0 0 0 6.4142e-08 -0.39370078 0 6.4142e-08
		 -0.39370078 0 6.4142e-08 -0.39370078 0 6.4142e-08 -0.39370078 0 -6.4142e-08 0.39370078
		 0 -6.4142e-08 0.39370078 0 -6.4142e-08 0.39370078 0 -6.4142e-08 0.39370078 0 -0.39370078
		 -6.4142e-08 0 -0.39370078 -6.4142e-08 0 -0.39370078 -6.4142e-08 0 -0.39370078 -6.4142e-08
		 0.39370078 0 0 0.39370078 0 0 0.39370078 0 0 0.39370078 0 0 0 0.39370078 6.4142e-08
		 0 0.39370078 6.4142e-08 0 0.39370078 6.4142e-08 0 0.39370078 6.4142e-08 -0.39370078
		 0 0 -0.39370078 0 0 -0.39370078 0 0 -0.39370078 0 0 0 6.4142e-08 -0.39370078 0 6.4142e-08
		 -0.39370078 0 6.4142e-08 -0.39370078 0 6.4142e-08 -0.39370078 0 -6.4142e-08 0.39370078
		 0 -6.4142e-08 0.39370078 0 -6.4142e-08 0.39370078 0 -6.4142e-08 0.39370078 0 -0.39370078
		 -6.4142e-08 0 -0.39370078 -6.4142e-08 0 -0.39370078 -6.4142e-08 0 -0.39370078 -6.4142e-08
		 0.39370078 0 0 0.39370078 0 0 0.39370078 0 0 0.39370078 0 0 0 0.39370078 6.4142e-08
		 0 0.39370078 6.4142e-08 0 0.39370078 6.4142e-08 0 0.39370078 6.4142e-08 -0.39370078
		 0 0 -0.39370078 0 0 -0.39370078 0 0 -0.39370078 0 0 0 6.4142e-08 -0.39370078 0 6.4142e-08
		 -0.39370078 0 6.4142e-08 -0.39370078 0 6.4142e-08 -0.39370078 0 -6.4142e-08 0.39370078
		 0 -6.4142e-08 0.39370078 0 -6.4142e-08 0.39370078 0 -6.4142e-08 0.39370078 0 -0.39370078
		 -6.4142e-08 0 -0.39370078 -6.4142e-08 0 -0.39370078 -6.4142e-08 0 -0.39370078 -6.4142e-08
		 0.39370078 0 0 0.39370078 0 0 0.39370078 0 0 0.39370078 0 0 0 0.39370078 6.4142e-08
		 0 0.39370078 6.4142e-08 0 0.39370078 6.4142e-08 0 0.39370078 6.4142e-08 -0.39370078
		 0 0 -0.39370078 0 0 -0.39370078 0 0 -0.39370078 0 0 0 6.4142e-08 -0.39370078 0 6.4142e-08
		 -0.39370078 0 6.4142e-08 -0.39370078 0 6.4142e-08 -0.39370078 0 -6.4142e-08 0.39370078
		 0 -6.4142e-08 0.39370078 0 -6.4142e-08 0.39370078 0 -6.4142e-08 0.39370078 0 -0.39370078
		 -6.4142e-08 0 -0.39370078 -6.4142e-08 0 -0.39370078 -6.4142e-08 0 -0.39370078 -6.4142e-08
		 0.39370078 0 0 0.39370078 0 0 0.39370078 0 0 0.39370078 0 0 0 0.39370078 6.4142e-08
		 0 0.39370078 6.4142e-08 0 0.39370078 6.4142e-08 0 0.39370078 6.4142e-08 -0.39370078
		 0 0 -0.39370078 0 0 -0.39370078 0 0 -0.39370078 0 0 0 6.4142e-08 -0.39370078 0 6.4142e-08
		 -0.39370078 0 6.4142e-08 -0.39370078 0 6.4142e-08 -0.39370078 0 -6.4142e-08 0.39370078
		 0 -6.4142e-08 0.39370078 0 -6.4142e-08 0.39370078 0 -6.4142e-08 0.39370078 0 -0.39370078
		 -6.4142e-08 0 -0.39370078 -6.4142e-08 0 -0.39370078 -6.4142e-08 0 -0.39370078 -6.4142e-08
		 0.39370078 0 0 0.39370078 0 0 0.39370078 0 0 0.39370078 0 0 0 0.39370078 6.4142e-08
		 0 0.39370078 6.4142e-08 0 0.39370078 6.4142e-08 0 0.39370078 6.4142e-08 -0.39370078
		 0 0 -0.39370078 0 0 -0.39370078 0 0 -0.39370078 0 0 0 5.3209742e-08 -0.32659906 0
		 5.3209742e-08 -0.32659906 0 5.3209742e-08 -0.32659906 0 5.3209742e-08 -0.32659906
		 0 -5.3209742e-08 0.32659906 0 -5.3209742e-08 0.32659906 0 -5.3209742e-08 0.32659906
		 0 -5.3209742e-08 0.32659906 0 -0.26246721 -4.2761336e-08 0 -0.26246721 -4.2761336e-08
		 0 -0.26246721 -4.2761336e-08 0 -0.26246721 -4.2761336e-08 0.26246721 0 0 0.26246721
		 0 0 0.26246721 0 0 0.26246721 0 0 0 0.26246721 4.2761336e-08 0 0.26246721 4.2761336e-08
		 0 0.26246721 4.2761336e-08 0 0.26246721 4.2761336e-08 -0.26246721 0 0 -0.26246721
		 0 0;
	setAttr ".n[166:287]" -type "float3"  -0.26246721 0 0 -0.26246721 0 0 0 5.3209742e-08
		 -0.32659906 0 5.3209742e-08 -0.32659906 0 5.3209742e-08 -0.32659906 0 5.3209742e-08
		 -0.32659906 0 -5.3209742e-08 0.32659906 0 -5.3209742e-08 0.32659906 0 -5.3209742e-08
		 0.32659906 0 -5.3209742e-08 0.32659906 0 -0.26246721 -5.6983396e-08 0 -0.26246721
		 -5.6983396e-08 0 -0.26246721 -5.6983396e-08 0 -0.26246721 -5.6983396e-08 0.26246721
		 0 0 0.26246721 0 0 0.26246721 0 0 0.26246721 0 0 0 0.26246721 4.2761336e-08 0 0.26246721
		 4.2761336e-08 0 0.26246721 4.2761336e-08 0 0.26246721 4.2761336e-08 -0.26246721 0
		 0 -0.26246721 0 0 -0.26246721 0 0 -0.26246721 0 0 0 1.032714e-07 -0.32659906 0 1.032714e-07
		 -0.32659906 0 1.032714e-07 -0.32659906 0 1.032714e-07 -0.32659906 0 -1.5333305e-07
		 0.32659906 0 -1.5333305e-07 0.32659906 0 -1.5333305e-07 0.32659906 0 -1.5333305e-07
		 0.32659906 0 -0.26246721 -4.2761336e-08 0 -0.26246721 -4.2761336e-08 0 -0.26246721
		 -4.2761336e-08 0 -0.26246721 -4.2761336e-08 0.26246721 0 0 0.26246718 0 0 0.26246718
		 0 0 0.26246721 0 0 0 0.26246721 4.2761336e-08 0 0.26246721 4.2761336e-08 0 0.26246721
		 4.2761336e-08 0 0.26246721 4.2761336e-08 -0.26246721 0 0 -0.26246721 0 0 -0.26246721
		 0 0 -0.26246721 0 0 0 1.032714e-07 -0.32659906 0 1.032714e-07 -0.32659906 0 1.032714e-07
		 -0.32659906 0 1.032714e-07 -0.32659906 0 -1.5333305e-07 0.32659906 0 -1.5333305e-07
		 0.32659906 0 -1.5333305e-07 0.32659906 0 -1.5333305e-07 0.32659906 0 -0.26246721
		 -4.2761336e-08 0 -0.26246721 -4.2761336e-08 0 -0.26246721 -4.2761336e-08 0 -0.26246721
		 -4.2761336e-08 0.26246721 0 0 0.26246718 0 0 0.26246718 0 0 0.26246721 0 0 0 0.26246721
		 4.2761336e-08 0 0.26246721 4.2761336e-08 0 0.26246721 4.2761336e-08 0 0.26246721
		 4.2761336e-08 -0.26246721 0 0 -0.26246721 0 0 -0.26246721 0 0 -0.26246721 0 0 0 9.2320413e-08
		 -0.32659906 0 9.2320413e-08 -0.32659906 0 9.2320413e-08 -0.32659906 0 9.2320413e-08
		 -0.32659906 0 -1.0327139e-07 0.32659906 0 -1.0327139e-07 0.32659906 0 -1.0327139e-07
		 0.32659906 0 -1.0327139e-07 0.32659906 0 -0.26246721 -4.2761336e-08 0 -0.26246721
		 -4.2761336e-08 0 -0.26246721 -4.2761336e-08 0 -0.26246721 -4.2761336e-08 0.26246721
		 0 0 0.26246721 0 0 0.26246721 0 0 0.26246721 0 0 0 0.26246721 4.2761336e-08 0 0.26246721
		 4.2761336e-08 0 0.26246721 4.2761336e-08 0 0.26246721 4.2761336e-08 -0.26246718 0
		 0 -0.26246721 0 0 -0.26246718 0 0 -0.26246721 0 0 0 9.2320413e-08 -0.32659906 0 9.2320413e-08
		 -0.32659906 0 9.2320413e-08 -0.32659906 0 9.2320413e-08 -0.32659906 0 -1.0327139e-07
		 0.32659906 0 -1.0327139e-07 0.32659906 0 -1.0327139e-07 0.32659906 0 -1.0327139e-07
		 0.32659906 0 -0.26246721 -4.2761336e-08 0 -0.26246721 -4.2761336e-08 0 -0.26246721
		 -4.2761336e-08 0 -0.26246721 -4.2761336e-08 0.26246721 0 0 0.26246721 0 0 0.26246721
		 0 0 0.26246721 0 0 0 0.26246721 4.2761336e-08 0 0.26246721 4.2761336e-08 0 0.26246721
		 4.2761336e-08 0 0.26246721 4.2761336e-08 -0.26246718 0 0 -0.26246721 0 0 -0.26246718
		 0 0 -0.26246721 0 0;
	setAttr -s 72 -ch 288 ".fc[0:71]" -type "polyFaces" 
		f 4 0 1 2 3
		mu 0 4 1 0 4 15
		f 4 4 5 6 7
		mu 0 4 2 9 10 3
		f 4 -4 8 -5 9
		mu 0 4 13 14 18 19
		f 4 -3 10 -6 -9
		mu 0 4 12 11 22 23
		f 4 -2 11 -7 -11
		mu 0 4 8 5 6 7
		f 4 -1 -10 -8 -12
		mu 0 4 16 17 20 21
		f 4 12 13 14 15
		mu 0 4 44 24 28 45
		f 4 16 17 18 19
		mu 0 4 29 33 34 32
		f 4 -16 20 -17 21
		mu 0 4 25 39 42 43
		f 4 -15 22 -18 -21
		mu 0 4 38 37 46 47
		f 4 -14 23 -19 -23
		mu 0 4 41 40 30 31
		f 4 -13 -22 -20 -24
		mu 0 4 35 36 26 27
		f 4 24 25 26 27
		mu 0 4 65 64 68 69
		f 4 28 29 30 31
		mu 0 4 53 57 58 56
		f 4 -28 32 -29 33
		mu 0 4 49 63 66 67
		f 4 -27 34 -30 -33
		mu 0 4 62 61 70 71
		f 4 -26 35 -31 -35
		mu 0 4 52 48 54 55
		f 4 -25 -34 -32 -36
		mu 0 4 59 60 50 51
		f 4 36 37 38 39
		mu 0 4 89 88 92 93
		f 4 40 41 42 43
		mu 0 4 77 81 82 80
		f 4 -40 44 -41 45
		mu 0 4 73 87 90 91
		f 4 -39 46 -42 -45
		mu 0 4 86 85 94 95
		f 4 -38 47 -43 -47
		mu 0 4 76 72 78 79
		f 4 -37 -46 -44 -48
		mu 0 4 83 84 74 75
		f 4 48 49 50 51
		mu 0 4 113 112 116 117
		f 4 52 53 54 55
		mu 0 4 101 105 106 104
		f 4 -52 56 -53 57
		mu 0 4 97 111 114 115
		f 4 -51 58 -54 -57
		mu 0 4 110 109 118 119
		f 4 -50 59 -55 -59
		mu 0 4 100 96 102 103
		f 4 -49 -58 -56 -60
		mu 0 4 107 108 98 99
		f 4 60 61 62 63
		mu 0 4 137 136 140 141
		f 4 64 65 66 67
		mu 0 4 125 129 130 128
		f 4 -64 68 -65 69
		mu 0 4 121 135 138 139
		f 4 -63 70 -66 -69
		mu 0 4 134 133 142 143
		f 4 -62 71 -67 -71
		mu 0 4 124 120 126 127
		f 4 -61 -70 -68 -72
		mu 0 4 131 132 122 123
		f 4 72 73 74 75
		mu 0 4 144 145 146 147
		f 4 76 77 78 79
		mu 0 4 148 149 150 151
		f 4 -76 80 -77 81
		mu 0 4 152 153 154 155
		f 4 -75 82 -78 -81
		mu 0 4 156 157 158 159
		f 4 -74 83 -79 -83
		mu 0 4 160 161 162 163
		f 4 -73 -82 -80 -84
		mu 0 4 164 165 166 167
		f 4 84 85 86 87
		mu 0 4 168 169 170 171
		f 4 88 89 90 91
		mu 0 4 172 173 174 175
		f 4 -88 92 -89 93
		mu 0 4 176 177 178 179
		f 4 -87 94 -90 -93
		mu 0 4 180 181 182 183
		f 4 -86 95 -91 -95
		mu 0 4 184 185 186 187
		f 4 -85 -94 -92 -96
		mu 0 4 188 189 190 191
		f 4 96 97 98 99
		mu 0 4 192 193 194 195
		f 4 100 101 102 103
		mu 0 4 196 197 198 199
		f 4 -100 104 -101 105
		mu 0 4 200 201 202 203
		f 4 -99 106 -102 -105
		mu 0 4 204 205 206 207
		f 4 -98 107 -103 -107
		mu 0 4 208 209 210 211
		f 4 -97 -106 -104 -108
		mu 0 4 212 213 214 215
		f 4 108 109 110 111
		mu 0 4 216 217 218 219
		f 4 112 113 114 115
		mu 0 4 220 221 222 223
		f 4 -112 116 -113 117
		mu 0 4 224 225 226 227
		f 4 -111 118 -114 -117
		mu 0 4 228 229 230 231
		f 4 -110 119 -115 -119
		mu 0 4 232 233 234 235
		f 4 -109 -118 -116 -120
		mu 0 4 236 237 238 239
		f 4 120 121 122 123
		mu 0 4 240 241 242 243
		f 4 124 125 126 127
		mu 0 4 244 245 246 247
		f 4 -124 128 -125 129
		mu 0 4 248 249 250 251
		f 4 -123 130 -126 -129
		mu 0 4 252 253 254 255
		f 4 -122 131 -127 -131
		mu 0 4 256 257 258 259
		f 4 -121 -130 -128 -132
		mu 0 4 260 261 262 263
		f 4 132 133 134 135
		mu 0 4 264 265 266 267
		f 4 136 137 138 139
		mu 0 4 268 269 270 271
		f 4 -136 140 -137 141
		mu 0 4 272 273 274 275
		f 4 -135 142 -138 -141
		mu 0 4 276 277 278 279
		f 4 -134 143 -139 -143
		mu 0 4 280 281 282 283
		f 4 -133 -142 -140 -144
		mu 0 4 284 285 286 287;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
	setAttr ".bw" 1.023018;
createNode lightLinker -s -n "lightLinker1";
	rename -uid "7B7855A7-45DC-5CD9-DABD-76981F3EACC7";
	setAttr -s 4 ".lnk";
	setAttr -s 4 ".slnk";
createNode shapeEditorManager -n "shapeEditorManager";
	rename -uid "40F732DD-4430-9FFB-087D-1EBF4FB3E08C";
createNode poseInterpolatorManager -n "poseInterpolatorManager";
	rename -uid "7F70AB22-4D5E-CCD9-1403-0ABCC1DE672C";
createNode displayLayerManager -n "layerManager";
	rename -uid "960A1F49-40E2-7B0F-8344-959A0DC1E576";
createNode displayLayer -n "defaultLayer";
	rename -uid "9090ADF3-400F-E42B-789A-E08CFD1BC507";
createNode renderLayerManager -n "renderLayerManager";
	rename -uid "6B1C7225-4C07-3149-E413-38AF9ED91FEC";
createNode renderLayer -n "defaultRenderLayer";
	rename -uid "C889E30A-4FE7-AF58-5F46-8B89E0968ABC";
	setAttr ".g" yes;
createNode phong -n "Sheep_ncl1_1";
	rename -uid "6BEB50AD-40F3-9E81-1575-D1A967ECBC22";
	setAttr ".dc" 1;
	setAttr ".ambc" -type "float3" 0.58823532 0.58823532 0.58823532 ;
	setAttr ".sc" -type "float3" 0 0 0 ;
	setAttr ".rfl" 1;
	setAttr ".cp" 2;
createNode shadingEngine -n "SheepSG";
	rename -uid "22419D67-4EDF-8271-C326-249C07F9782C";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo1";
	rename -uid "31D10683-4B64-58AF-5B46-D28845CCE0C6";
createNode file -n "MapFBXASC032FBXASC0352";
	rename -uid "34AD9CBD-401F-CF63-728C-FC89F5B8299B";
	setAttr ".ftn" -type "string" "C:\\Users\\vince\\Desktop\\Vincent\\3D\\Minecraft\\Sheep\\sheep.png";
	setAttr ".cs" -type "string" "sRGB";
createNode place2dTexture -n "place2dTexture1";
	rename -uid "7259D78B-42DC-C087-BF9F-0D90BB93D7F3";
createNode phong -n "SheepFBXASC032Fur_ncl1_1";
	rename -uid "35E4A9C4-4705-7F61-CC47-FCA671486AC2";
	setAttr ".dc" 1;
	setAttr ".ambc" -type "float3" 0.58823532 0.58823532 0.58823532 ;
	setAttr ".sc" -type "float3" 0 0 0 ;
	setAttr ".rfl" 1;
	setAttr ".cp" 2;
createNode shadingEngine -n "SheepFBXASC032FurSG";
	rename -uid "FEE3C1ED-40E2-3C8A-32D3-4E8944BD6569";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo2";
	rename -uid "A19CA333-4D3E-A344-6194-11BA382AE5AC";
createNode file -n "MapFBXASC032FBXASC0354";
	rename -uid "4B2AF3BF-4A6C-5175-39C4-BD85E856B0C6";
	setAttr ".ftn" -type "string" "C:\\Users\\vince\\Desktop\\Vincent\\3D\\Minecraft\\Sheep\\sheep_fur.png";
	setAttr ".cs" -type "string" "sRGB";
createNode place2dTexture -n "place2dTexture2";
	rename -uid "2B9768C2-43FF-FCC3-120B-31A1B2565CD6";
createNode script -n "uiConfigurationScriptNode";
	rename -uid "63DFDAD2-4C7E-E28F-0FF0-76A9FE7AB006";
	setAttr ".b" -type "string" (
		"// Maya Mel UI Configuration File.\n//\n//  This script is machine generated.  Edit at your own risk.\n//\n//\n\nglobal string $gMainPane;\nif (`paneLayout -exists $gMainPane`) {\n\n\tglobal int $gUseScenePanelConfig;\n\tint    $useSceneConfig = $gUseScenePanelConfig;\n\tint    $nodeEditorPanelVisible = stringArrayContains(\"nodeEditorPanel1\", `getPanel -vis`);\n\tint    $nodeEditorWorkspaceControlOpen = (`workspaceControl -exists nodeEditorPanel1Window` && `workspaceControl -q -visible nodeEditorPanel1Window`);\n\tint    $menusOkayInPanels = `optionVar -q allowMenusInPanels`;\n\tint    $nVisPanes = `paneLayout -q -nvp $gMainPane`;\n\tint    $nPanes = 0;\n\tstring $editorName;\n\tstring $panelName;\n\tstring $itemFilterName;\n\tstring $panelConfig;\n\n\t//\n\t//  get current state of the UI\n\t//\n\tsceneUIReplacement -update $gMainPane;\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Top View\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Top View\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"|top\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"smoothShaded\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 0\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 32768\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n"
		+ "            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n            -rendererName \"vp2Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -controllers 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n"
		+ "            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            -captureSequenceNumber -1\n            -width 1\n            -height 1\n            -sceneRenderFilter 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n"
		+ "\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Side View\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Side View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"|side\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"smoothShaded\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 0\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n"
		+ "            -textureDisplay \"modulate\" \n            -textureMaxSize 32768\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n            -rendererName \"vp2Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n"
		+ "            -sortTransparent 1\n            -controllers 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            -captureSequenceNumber -1\n            -width 1\n            -height 1\n"
		+ "            -sceneRenderFilter 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Front View\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Front View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"|front\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"smoothShaded\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 0\n            -backfaceCulling 0\n"
		+ "            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 32768\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n            -rendererName \"vp2Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n"
		+ "            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -controllers 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n"
		+ "            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            -captureSequenceNumber -1\n            -width 1\n            -height 1\n            -sceneRenderFilter 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Persp View\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Persp View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"|persp\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"smoothShaded\" \n            -activeOnly 0\n            -ignorePanZoom 0\n"
		+ "            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 0\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 1\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 32768\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n            -rendererName \"vp2Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n"
		+ "            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -controllers 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n"
		+ "            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            -captureSequenceNumber -1\n            -width 639\n            -height 841\n            -sceneRenderFilter 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"outlinerPanel\" (localizedPanelLabel(\"ToggledOutliner\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\toutlinerPanel -edit -l (localizedPanelLabel(\"ToggledOutliner\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\t$editorName = $panelName;\n        outlinerEditor -e \n            -docTag \"isolOutln_fromSeln\" \n            -showShapes 0\n            -showAssignedMaterials 0\n            -showTimeEditor 1\n            -showReferenceNodes 1\n            -showReferenceMembers 1\n            -showAttributes 0\n            -showConnected 0\n            -showAnimCurvesOnly 0\n            -showMuteInfo 0\n            -organizeByLayer 1\n            -organizeByClip 1\n            -showAnimLayerWeight 1\n            -autoExpandLayers 1\n            -autoExpand 0\n            -showDagOnly 1\n            -showAssets 1\n            -showContainedOnly 1\n            -showPublishedAsConnected 0\n            -showParentContainers 0\n            -showContainerContents 1\n            -ignoreDagHierarchy 0\n            -expandConnections 0\n            -showUpstreamCurves 1\n            -showUnitlessCurves 1\n            -showCompounds 1\n            -showLeafs 1\n            -showNumericAttrsOnly 0\n            -highlightActive 1\n            -autoSelectNewObjects 0\n"
		+ "            -doNotSelectNewObjects 0\n            -dropIsParent 1\n            -transmitFilters 0\n            -setFilter \"defaultSetFilter\" \n            -showSetMembers 1\n            -allowMultiSelection 1\n            -alwaysToggleSelect 0\n            -directSelect 0\n            -isSet 0\n            -isSetMember 0\n            -displayMode \"DAG\" \n            -expandObjects 0\n            -setsIgnoreFilters 1\n            -containersIgnoreFilters 0\n            -editAttrName 0\n            -showAttrValues 0\n            -highlightSecondary 0\n            -showUVAttrsOnly 0\n            -showTextureNodesOnly 0\n            -attrAlphaOrder \"default\" \n            -animLayerFilterOptions \"allAffecting\" \n            -sortOrder \"none\" \n            -longNames 0\n            -niceNames 1\n            -selectCommand \"print(\\\"\\\")\" \n            -showNamespace 1\n            -showPinIcons 0\n            -mapMotionTrails 0\n            -ignoreHiddenAttribute 0\n            -ignoreOutlinerColor 0\n            -renderFilterVisible 0\n            -renderFilterIndex 0\n"
		+ "            -selectionOrder \"chronological\" \n            -expandAttribute 0\n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"outlinerPanel\" (localizedPanelLabel(\"Outliner\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\toutlinerPanel -edit -l (localizedPanelLabel(\"Outliner\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        outlinerEditor -e \n            -showShapes 0\n            -showAssignedMaterials 0\n            -showTimeEditor 1\n            -showReferenceNodes 0\n            -showReferenceMembers 0\n            -showAttributes 0\n            -showConnected 0\n            -showAnimCurvesOnly 0\n            -showMuteInfo 0\n            -organizeByLayer 1\n            -organizeByClip 1\n            -showAnimLayerWeight 1\n            -autoExpandLayers 1\n            -autoExpand 0\n            -showDagOnly 1\n            -showAssets 1\n            -showContainedOnly 1\n            -showPublishedAsConnected 0\n"
		+ "            -showParentContainers 0\n            -showContainerContents 1\n            -ignoreDagHierarchy 0\n            -expandConnections 0\n            -showUpstreamCurves 1\n            -showUnitlessCurves 1\n            -showCompounds 1\n            -showLeafs 1\n            -showNumericAttrsOnly 0\n            -highlightActive 1\n            -autoSelectNewObjects 0\n            -doNotSelectNewObjects 0\n            -dropIsParent 1\n            -transmitFilters 0\n            -setFilter \"defaultSetFilter\" \n            -showSetMembers 1\n            -allowMultiSelection 1\n            -alwaysToggleSelect 0\n            -directSelect 0\n            -displayMode \"DAG\" \n            -expandObjects 0\n            -setsIgnoreFilters 1\n            -containersIgnoreFilters 0\n            -editAttrName 0\n            -showAttrValues 0\n            -highlightSecondary 0\n            -showUVAttrsOnly 0\n            -showTextureNodesOnly 0\n            -attrAlphaOrder \"default\" \n            -animLayerFilterOptions \"allAffecting\" \n            -sortOrder \"none\" \n"
		+ "            -longNames 0\n            -niceNames 1\n            -showNamespace 1\n            -showPinIcons 0\n            -mapMotionTrails 0\n            -ignoreHiddenAttribute 0\n            -ignoreOutlinerColor 0\n            -renderFilterVisible 0\n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"graphEditor\" (localizedPanelLabel(\"Graph Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Graph Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAssignedMaterials 0\n                -showTimeEditor 1\n                -showReferenceNodes 0\n                -showReferenceMembers 0\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n"
		+ "                -organizeByClip 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 1\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showParentContainers 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 1\n                -showCompounds 0\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 1\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 1\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n"
		+ "                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 1\n                -mapMotionTrails 1\n                -ignoreHiddenAttribute 0\n                -ignoreOutlinerColor 0\n                -renderFilterVisible 0\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"GraphEd\");\n            animCurveEditor -e \n                -displayValues 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -showPlayRangeShades \"on\" \n                -lockPlayRangeShades \"off\" \n                -smoothness \"fine\" \n                -resultSamples 1\n"
		+ "                -resultScreenSamples 0\n                -resultUpdate \"delayed\" \n                -showUpstreamCurves 1\n                -keyMinScale 1\n                -stackedCurvesMin -1\n                -stackedCurvesMax 1\n                -stackedCurvesSpace 0.2\n                -preSelectionHighlight 0\n                -constrainDrag 0\n                -valueLinesToggle 1\n                -highlightAffectedCurves 0\n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dopeSheetPanel\" (localizedPanelLabel(\"Dope Sheet\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Dope Sheet\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAssignedMaterials 0\n                -showTimeEditor 1\n                -showReferenceNodes 0\n                -showReferenceMembers 0\n"
		+ "                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -organizeByClip 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showParentContainers 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 0\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 1\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"0\" \n                -showSetMembers 0\n"
		+ "                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n                -mapMotionTrails 1\n                -ignoreHiddenAttribute 0\n                -ignoreOutlinerColor 0\n                -renderFilterVisible 0\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"DopeSheetEd\");\n            dopeSheetEditor -e \n                -displayValues 0\n                -snapTime \"integer\" \n"
		+ "                -snapValue \"none\" \n                -outliner \"dopeSheetPanel1OutlineEd\" \n                -showSummary 1\n                -showScene 0\n                -hierarchyBelow 0\n                -showTicks 1\n                -selectionWindow 0 0 0 0 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"timeEditorPanel\" (localizedPanelLabel(\"Time Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Time Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"clipEditorPanel\" (localizedPanelLabel(\"Trax Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Trax Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = clipEditorNameFromPanel($panelName);\n"
		+ "            clipEditor -e \n                -displayValues 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -initialized 0\n                -manageSequencer 0 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"sequenceEditorPanel\" (localizedPanelLabel(\"Camera Sequencer\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Camera Sequencer\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = sequenceEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayValues 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -initialized 0\n                -manageSequencer 1 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"hyperGraphPanel\" (localizedPanelLabel(\"Hypergraph Hierarchy\")) `;\n"
		+ "\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Hypergraph Hierarchy\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"HyperGraphEd\");\n            hyperGraph -e \n                -graphLayoutStyle \"hierarchicalLayout\" \n                -orientation \"horiz\" \n                -mergeConnections 0\n                -zoom 1\n                -animateTransition 0\n                -showRelationships 1\n                -showShapes 0\n                -showDeformers 0\n                -showExpressions 0\n                -showConstraints 0\n                -showConnectionFromSelected 0\n                -showConnectionToSelected 0\n                -showConstraintLabels 0\n                -showUnderworld 0\n                -showInvisible 0\n                -transitionFrames 1\n                -opaqueContainers 0\n                -freeform 0\n                -imagePosition 0 0 \n                -imageScale 1\n                -imageEnabled 0\n                -graphType \"DAG\" \n"
		+ "                -heatMapDisplay 0\n                -updateSelection 1\n                -updateNodeAdded 1\n                -useDrawOverrideColor 0\n                -limitGraphTraversal -1\n                -range 0 0 \n                -iconSize \"smallIcons\" \n                -showCachedConnections 0\n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"hyperShadePanel\" (localizedPanelLabel(\"Hypershade\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Hypershade\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"visorPanel\" (localizedPanelLabel(\"Visor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Visor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n"
		+ "\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"nodeEditorPanel\" (localizedPanelLabel(\"Node Editor\")) `;\n\tif ($nodeEditorPanelVisible || $nodeEditorWorkspaceControlOpen) {\n\t\tif (\"\" == $panelName) {\n\t\t\tif ($useSceneConfig) {\n\t\t\t\t$panelName = `scriptedPanel -unParent  -type \"nodeEditorPanel\" -l (localizedPanelLabel(\"Node Editor\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"NodeEditorEd\");\n            nodeEditor -e \n                -allAttributes 0\n                -allNodes 0\n                -autoSizeNodes 1\n                -consistentNameSize 1\n                -createNodeCommand \"nodeEdCreateNodeCommand\" \n                -connectNodeOnCreation 0\n                -connectOnDrop 0\n                -copyConnectionsOnPaste 0\n                -connectionStyle \"bezier\" \n                -defaultPinnedState 0\n                -additiveGraphingMode 0\n                -settingsChangedCallback \"nodeEdSyncControls\" \n                -traversalDepthLimit -1\n                -keyPressCommand \"nodeEdKeyPressCommand\" \n"
		+ "                -nodeTitleMode \"name\" \n                -gridSnap 0\n                -gridVisibility 1\n                -crosshairOnEdgeDragging 0\n                -popupMenuScript \"nodeEdBuildPanelMenus\" \n                -showNamespace 1\n                -showShapes 1\n                -showSGShapes 0\n                -showTransforms 1\n                -useAssets 1\n                -syncedSelection 1\n                -extendToShapes 1\n                -editorMode \"default\" \n                -hasWatchpoint 0\n                $editorName;\n\t\t\t}\n\t\t} else {\n\t\t\t$label = `panel -q -label $panelName`;\n\t\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Node Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"NodeEditorEd\");\n            nodeEditor -e \n                -allAttributes 0\n                -allNodes 0\n                -autoSizeNodes 1\n                -consistentNameSize 1\n                -createNodeCommand \"nodeEdCreateNodeCommand\" \n                -connectNodeOnCreation 0\n                -connectOnDrop 0\n"
		+ "                -copyConnectionsOnPaste 0\n                -connectionStyle \"bezier\" \n                -defaultPinnedState 0\n                -additiveGraphingMode 0\n                -settingsChangedCallback \"nodeEdSyncControls\" \n                -traversalDepthLimit -1\n                -keyPressCommand \"nodeEdKeyPressCommand\" \n                -nodeTitleMode \"name\" \n                -gridSnap 0\n                -gridVisibility 1\n                -crosshairOnEdgeDragging 0\n                -popupMenuScript \"nodeEdBuildPanelMenus\" \n                -showNamespace 1\n                -showShapes 1\n                -showSGShapes 0\n                -showTransforms 1\n                -useAssets 1\n                -syncedSelection 1\n                -extendToShapes 1\n                -editorMode \"default\" \n                -hasWatchpoint 0\n                $editorName;\n\t\t\tif (!$useSceneConfig) {\n\t\t\t\tpanel -e -l $label $panelName;\n\t\t\t}\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"createNodePanel\" (localizedPanelLabel(\"Create Node\")) `;\n"
		+ "\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Create Node\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"polyTexturePlacementPanel\" (localizedPanelLabel(\"UV Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"UV Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"renderWindowPanel\" (localizedPanelLabel(\"Render View\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Render View\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"shapePanel\" (localizedPanelLabel(\"Shape Editor\")) `;\n"
		+ "\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tshapePanel -edit -l (localizedPanelLabel(\"Shape Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"posePanel\" (localizedPanelLabel(\"Pose Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tposePanel -edit -l (localizedPanelLabel(\"Pose Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dynRelEdPanel\" (localizedPanelLabel(\"Dynamic Relationships\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Dynamic Relationships\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"relationshipPanel\" (localizedPanelLabel(\"Relationship Editor\")) `;\n"
		+ "\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Relationship Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"referenceEditorPanel\" (localizedPanelLabel(\"Reference Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Reference Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"componentEditorPanel\" (localizedPanelLabel(\"Component Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Component Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dynPaintScriptedPanelType\" (localizedPanelLabel(\"Paint Effects\")) `;\n"
		+ "\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Paint Effects\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"scriptEditorPanel\" (localizedPanelLabel(\"Script Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Script Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"profilerPanel\" (localizedPanelLabel(\"Profiler Tool\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Profiler Tool\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"contentBrowserPanel\" (localizedPanelLabel(\"Content Browser\")) `;\n"
		+ "\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Content Browser\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\tif ($useSceneConfig) {\n        string $configName = `getPanel -cwl (localizedPanelLabel(\"Current Layout\"))`;\n        if (\"\" != $configName) {\n\t\t\tpanelConfiguration -edit -label (localizedPanelLabel(\"Current Layout\")) \n\t\t\t\t-userCreated false\n\t\t\t\t-defaultImage \"vacantCell.xP:/\"\n\t\t\t\t-image \"\"\n\t\t\t\t-sc false\n\t\t\t\t-configString \"global string $gMainPane; paneLayout -e -cn \\\"single\\\" -ps 1 100 100 $gMainPane;\"\n\t\t\t\t-removeAllPanels\n\t\t\t\t-ap false\n\t\t\t\t\t(localizedPanelLabel(\"Persp View\")) \n\t\t\t\t\t\"modelPanel\"\n"
		+ "\t\t\t\t\t\"$panelName = `modelPanel -unParent -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels `;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"smoothShaded\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -holdOuts 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 0\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 1\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 32768\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -depthOfFieldPreview 1\\n    -maxConstantTransparency 1\\n    -rendererName \\\"vp2Renderer\\\" \\n    -objectFilterShowInHUD 1\\n    -isFiltered 0\\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -controllers 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -imagePlane 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -particleInstancers 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -pluginShapes 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -clipGhosts 1\\n    -greasePencils 1\\n    -shadows 0\\n    -captureSequenceNumber -1\\n    -width 639\\n    -height 841\\n    -sceneRenderFilter 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName;\\nmodelEditor -e \\n    -pluginObjects \\\"gpuCacheDisplayFilter\\\" 1 \\n    $editorName\"\n"
		+ "\t\t\t\t\t\"modelPanel -edit -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels  $panelName;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"smoothShaded\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -holdOuts 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 0\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 1\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 32768\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -depthOfFieldPreview 1\\n    -maxConstantTransparency 1\\n    -rendererName \\\"vp2Renderer\\\" \\n    -objectFilterShowInHUD 1\\n    -isFiltered 0\\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -controllers 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -imagePlane 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -particleInstancers 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -pluginShapes 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -clipGhosts 1\\n    -greasePencils 1\\n    -shadows 0\\n    -captureSequenceNumber -1\\n    -width 639\\n    -height 841\\n    -sceneRenderFilter 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName;\\nmodelEditor -e \\n    -pluginObjects \\\"gpuCacheDisplayFilter\\\" 1 \\n    $editorName\"\n"
		+ "\t\t\t\t$configName;\n\n            setNamedPanelLayout (localizedPanelLabel(\"Current Layout\"));\n        }\n\n        panelHistory -e -clear mainPanelHistory;\n        sceneUIReplacement -clear;\n\t}\n\n\ngrid -spacing 10 -size 100 -divisions 1 -displayAxes yes -displayGridLines yes -displayDivisionLines yes -displayPerspectiveLabels no -displayOrthographicLabels no -displayAxesBold yes -perspectiveLabelPosition axis -orthographicLabelPosition edge;\nviewManip -drawCompass 0 -compassAngle 0 -frontParameters \"\" -homeParameters \"\" -selectionLockParameters \"\";\n}\n");
	setAttr ".st" 3;
createNode script -n "sceneConfigurationScriptNode";
	rename -uid "803D6EAC-4935-124D-421D-438DBD2A7B35";
	setAttr ".b" -type "string" "playbackOptions -min 1 -max 120 -ast 1 -aet 200 ";
	setAttr ".st" 6;
createNode file -n "file1";
	rename -uid "30302D7B-4600-5E2C-5157-88A326B9F7BE";
	setAttr ".ftn" -type "string" "C:/2DAE Extra Projects/Graphics Programming 2/GP2-Minecraft/Source Files/Sheep/textures/sheep_full.png";
	setAttr ".cs" -type "string" "sRGB";
createNode place2dTexture -n "place2dTexture3";
	rename -uid "EDD655F8-4551-9301-C5B6-47B8089CD1C6";
select -ne :time1;
	setAttr ".o" 1;
	setAttr ".unw" 1;
select -ne :hardwareRenderingGlobals;
	setAttr ".otfna" -type "stringArray" 22 "NURBS Curves" "NURBS Surfaces" "Polygons" "Subdiv Surface" "Particles" "Particle Instance" "Fluids" "Strokes" "Image Planes" "UI" "Lights" "Cameras" "Locators" "Joints" "IK Handles" "Deformers" "Motion Trails" "Components" "Hair Systems" "Follicles" "Misc. UI" "Ornaments"  ;
	setAttr ".otfva" -type "Int32Array" 22 0 1 1 1 1 1
		 1 1 1 0 0 0 0 0 0 0 0 0
		 0 0 0 0 ;
	setAttr ".fprt" yes;
select -ne :renderPartition;
	setAttr -s 4 ".st";
select -ne :renderGlobalsList1;
select -ne :defaultShaderList1;
	setAttr -s 7 ".s";
select -ne :postProcessList1;
	setAttr -s 2 ".p";
select -ne :defaultRenderUtilityList1;
	setAttr -s 3 ".u";
select -ne :defaultRenderingList1;
select -ne :defaultTextureList1;
	setAttr -s 3 ".tx";
select -ne :lambert1;
select -ne :initialShadingGroup;
	setAttr ".ro" yes;
select -ne :initialParticleSE;
	setAttr ".ro" yes;
select -ne :initialMaterialInfo;
select -ne :defaultRenderGlobals;
	addAttr -ci true -h true -sn "dss" -ln "defaultSurfaceShader" -dt "string";
	setAttr ".ren" -type "string" "arnold";
	setAttr ".dss" -type "string" "lambert1";
select -ne :defaultResolution;
	setAttr ".pa" 1;
select -ne :defaultColorMgtGlobals;
	setAttr ".cfe" yes;
	setAttr ".cfp" -type "string" "<MAYA_RESOURCES>/OCIO-configs/Maya2022-default/config.ocio";
	setAttr ".vtn" -type "string" "ACES 1.0 SDR-video (sRGB)";
	setAttr ".vn" -type "string" "ACES 1.0 SDR-video";
	setAttr ".dn" -type "string" "sRGB";
	setAttr ".wsn" -type "string" "ACEScg";
	setAttr ".otn" -type "string" "ACES 1.0 SDR-video (sRGB)";
	setAttr ".potn" -type "string" "ACES 1.0 SDR-video (sRGB)";
select -ne :hardwareRenderGlobals;
	setAttr ".ctrs" 256;
	setAttr ".btrs" 512;
relationship "link" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "SheepSG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "SheepFBXASC032FurSG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "SheepSG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "SheepFBXASC032FurSG.message" ":defaultLightSet.message";
connectAttr "layerManager.dli[0]" "defaultLayer.id";
connectAttr "renderLayerManager.rlmi[0]" "defaultRenderLayer.rlid";
connectAttr "MapFBXASC032FBXASC0352.oc" "Sheep_ncl1_1.c";
connectAttr "Sheep_ncl1_1.oc" "SheepSG.ss";
connectAttr "SheepSG.msg" "materialInfo1.sg";
connectAttr "Sheep_ncl1_1.msg" "materialInfo1.m";
connectAttr "MapFBXASC032FBXASC0352.msg" "materialInfo1.t" -na;
connectAttr "place2dTexture1.o" "MapFBXASC032FBXASC0352.uv";
connectAttr "place2dTexture1.ofu" "MapFBXASC032FBXASC0352.ofu";
connectAttr "place2dTexture1.ofv" "MapFBXASC032FBXASC0352.ofv";
connectAttr "place2dTexture1.rf" "MapFBXASC032FBXASC0352.rf";
connectAttr "place2dTexture1.reu" "MapFBXASC032FBXASC0352.reu";
connectAttr "place2dTexture1.rev" "MapFBXASC032FBXASC0352.rev";
connectAttr "place2dTexture1.vt1" "MapFBXASC032FBXASC0352.vt1";
connectAttr "place2dTexture1.vt2" "MapFBXASC032FBXASC0352.vt2";
connectAttr "place2dTexture1.vt3" "MapFBXASC032FBXASC0352.vt3";
connectAttr "place2dTexture1.vc1" "MapFBXASC032FBXASC0352.vc1";
connectAttr "place2dTexture1.ofs" "MapFBXASC032FBXASC0352.fs";
connectAttr ":defaultColorMgtGlobals.cme" "MapFBXASC032FBXASC0352.cme";
connectAttr ":defaultColorMgtGlobals.cfe" "MapFBXASC032FBXASC0352.cmcf";
connectAttr ":defaultColorMgtGlobals.cfp" "MapFBXASC032FBXASC0352.cmcp";
connectAttr ":defaultColorMgtGlobals.wsn" "MapFBXASC032FBXASC0352.ws";
connectAttr "MapFBXASC032FBXASC0354.oc" "SheepFBXASC032Fur_ncl1_1.c";
connectAttr "SheepFBXASC032Fur_ncl1_1.oc" "SheepFBXASC032FurSG.ss";
connectAttr "SheepFBXASC032FurSG.msg" "materialInfo2.sg";
connectAttr "SheepFBXASC032Fur_ncl1_1.msg" "materialInfo2.m";
connectAttr "MapFBXASC032FBXASC0354.msg" "materialInfo2.t" -na;
connectAttr "place2dTexture2.o" "MapFBXASC032FBXASC0354.uv";
connectAttr "place2dTexture2.ofu" "MapFBXASC032FBXASC0354.ofu";
connectAttr "place2dTexture2.ofv" "MapFBXASC032FBXASC0354.ofv";
connectAttr "place2dTexture2.rf" "MapFBXASC032FBXASC0354.rf";
connectAttr "place2dTexture2.reu" "MapFBXASC032FBXASC0354.reu";
connectAttr "place2dTexture2.rev" "MapFBXASC032FBXASC0354.rev";
connectAttr "place2dTexture2.vt1" "MapFBXASC032FBXASC0354.vt1";
connectAttr "place2dTexture2.vt2" "MapFBXASC032FBXASC0354.vt2";
connectAttr "place2dTexture2.vt3" "MapFBXASC032FBXASC0354.vt3";
connectAttr "place2dTexture2.vc1" "MapFBXASC032FBXASC0354.vc1";
connectAttr "place2dTexture2.ofs" "MapFBXASC032FBXASC0354.fs";
connectAttr ":defaultColorMgtGlobals.cme" "MapFBXASC032FBXASC0354.cme";
connectAttr ":defaultColorMgtGlobals.cfe" "MapFBXASC032FBXASC0354.cmcf";
connectAttr ":defaultColorMgtGlobals.cfp" "MapFBXASC032FBXASC0354.cmcp";
connectAttr ":defaultColorMgtGlobals.wsn" "MapFBXASC032FBXASC0354.ws";
connectAttr ":defaultColorMgtGlobals.cme" "file1.cme";
connectAttr ":defaultColorMgtGlobals.cfe" "file1.cmcf";
connectAttr ":defaultColorMgtGlobals.cfp" "file1.cmcp";
connectAttr ":defaultColorMgtGlobals.wsn" "file1.ws";
connectAttr "place2dTexture3.c" "file1.c";
connectAttr "place2dTexture3.tf" "file1.tf";
connectAttr "place2dTexture3.rf" "file1.rf";
connectAttr "place2dTexture3.mu" "file1.mu";
connectAttr "place2dTexture3.mv" "file1.mv";
connectAttr "place2dTexture3.s" "file1.s";
connectAttr "place2dTexture3.wu" "file1.wu";
connectAttr "place2dTexture3.wv" "file1.wv";
connectAttr "place2dTexture3.re" "file1.re";
connectAttr "place2dTexture3.of" "file1.of";
connectAttr "place2dTexture3.r" "file1.ro";
connectAttr "place2dTexture3.n" "file1.n";
connectAttr "place2dTexture3.vt1" "file1.vt1";
connectAttr "place2dTexture3.vt2" "file1.vt2";
connectAttr "place2dTexture3.vt3" "file1.vt3";
connectAttr "place2dTexture3.vc1" "file1.vc1";
connectAttr "place2dTexture3.o" "file1.uv";
connectAttr "place2dTexture3.ofs" "file1.fs";
connectAttr "SheepSG.pa" ":renderPartition.st" -na;
connectAttr "SheepFBXASC032FurSG.pa" ":renderPartition.st" -na;
connectAttr "Sheep_ncl1_1.msg" ":defaultShaderList1.s" -na;
connectAttr "SheepFBXASC032Fur_ncl1_1.msg" ":defaultShaderList1.s" -na;
connectAttr "place2dTexture1.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "place2dTexture2.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "place2dTexture3.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "defaultRenderLayer.msg" ":defaultRenderingList1.r" -na;
connectAttr "MapFBXASC032FBXASC0352.msg" ":defaultTextureList1.tx" -na;
connectAttr "MapFBXASC032FBXASC0354.msg" ":defaultTextureList1.tx" -na;
connectAttr "file1.msg" ":defaultTextureList1.tx" -na;
connectAttr "file1.oc" ":lambert1.c";
connectAttr "file1.ot" ":lambert1.it";
connectAttr "Sheep1Shape.iog" ":initialShadingGroup.dsm" -na;
connectAttr "file1.msg" ":initialMaterialInfo.t" -na;
// End of sheep.ma
