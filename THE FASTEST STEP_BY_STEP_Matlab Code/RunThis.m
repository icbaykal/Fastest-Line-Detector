load grayscale;
scrX=1300;scrY=780; 
imgRGB=ones(400,1230,3); 

scrsz = get(0,'ScreenSize'); 


hF1= figure(...
'Color',[0.9 0.9 0.9], ...
'Position',[25 20 scrX-40 scrY-40], ...
'MenuBar','none', ...
'Tag','Pattern Based Line Detector', ...
'Name','Line Detector',...
'Colormap',grayscale);

hAx1=axes( ...
'Units','normalized', ...
'Box','on', ...
'Color',[0.99 0.99 0.99], ... 
'Position',[0.3 0.377 0.6996 0.6235],...
'Visible','on');



hBO=uicontrol('Style','pushbutton', ...
'String','OPEN', ...
'FontSize',10, ...
'FontWeight','bold', ...
'BackgroundColor', [0.87 0.87 0.85],...
'Callback','Opencall(hAx1)', ...
'Units', 'Normal', ...
'Position',[0.0 0.97 0.1367 0.03]);
hBP=uicontrol('Style','pushbutton', ...
'String','EDGE DETECT', ...
'FontSize',10, ...
'FontWeight','bold', ...
'BackgroundColor', [0.87 0.87 0.85],...
'Callback','edgedet(hAx1)', ...
'Units','Normal', ...
'Position',[0.1367 0.97 0.1367 0.03]);

hBT=uicontrol('Style','pushbutton', ...
'String','PIECEWSE MATCH', ...
'FontSize',10, ...
'FontWeight','bold', ...
'BackgroundColor', [0.87 0.87 0.85],...
'Callback','piecewisematch(hAx1)', ...
'Units', 'Normal', ...
'Position',[0.0 0.94 0.1367 0.03]);
hBL=uicontrol('Style','pushbutton', ...
'String','SEGMENT', ...
'FontSize',10, ...
'FontWeight','bold', ...
'BackgroundColor', [0.87 0.87 0.85],...
'Callback','segments_lines(hAx1)', ...
'Units','Normal', ...
'Position',[0.1367 0.94 0.1367 0.03]);
hBT=uicontrol('Style','pushbutton', ...
'String','SHOW LINE', ...
'FontSize',10, ...
'FontWeight','bold', ...
'BackgroundColor', [0.87 0.87 0.85],...
'Callback','showline(hAx1)', ...
'Units', 'Normal', ...
'Position',[0.0 0.91 0.1367 0.03]);
hBL=uicontrol('Style','pushbutton', ...
'String','RESTART', ...
'FontSize',10, ...
'FontWeight','bold', ...
'BackgroundColor', [0.87 0.87 0.85],...
'Callback','restart_gui', ...
'Units','Normal', ...
'Position',[0.1367 0.91 0.1367 0.03]);
hVRFY=uicontrol('Style','pushbutton', ...
'String','VERIFY', ...
'FontSize',10, ...
'FontWeight','bold', ...
'BackgroundColor', [0.87 0.87 0.85],...
'Callback','verify(hAx1)', ...
'Units', 'Normal', ...
'Position',[0.0 0.88 0.1367 0.03]);

