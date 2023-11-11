<GameFile>
  <PropertyGroup Name="ToMau_CSB" Type="Layer" ID="ff12c41d-b15d-4b9f-b1a6-9c971cc4909c" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Layer" CustomClassName="ToMau" Tag="79" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="bg_1" ActionTag="-795817293" Tag="81" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="-338.0000" RightMargin="-338.0000" ctype="SpriteObjectData">
            <Size X="1700.0000" Y="768.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.6602" Y="1.0000" />
            <FileData Type="Normal" Path="games/ToMau/BG.png" Plist="" />
            <BlendFunc Src="1" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="root_layout" ActionTag="-356526763" Tag="80" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <Children>
              <AbstractNodeData Name="speaker" ActionTag="-912679806" CallBackType="Click" CallBackName="onClickSpeaker" Tag="142" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="48.0392" RightMargin="880.9608" TopMargin="31.2968" BottomMargin="641.7032" TouchEnable="True" FontSize="14" ButtonText="Button" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="65" Scale9Height="73" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="95.0000" Y="95.0000" />
                <Children>
                  <AbstractNodeData Name="loa" ActionTag="584706909" CallBackType="Click" Tag="143" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="14.0000" RightMargin="14.0000" TopMargin="15.5000" BottomMargin="15.5000" LeftEage="22" RightEage="22" TopEage="21" BottomEage="21" Scale9OriginX="22" Scale9OriginY="21" Scale9Width="23" Scale9Height="22" ctype="ImageViewObjectData">
                    <Size X="67.0000" Y="64.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="47.5000" Y="47.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.7053" Y="0.6737" />
                    <FileData Type="Normal" Path="games/ToMau/loa.png" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="95.5392" Y="689.2032" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.0933" Y="0.8974" />
                <PreSize X="0.0928" Y="0.1237" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                <NormalFileData Type="Normal" Path="games/ToMau/icon.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="question_box" ActionTag="552559205" Tag="137" IconVisible="False" LeftMargin="164.6963" RightMargin="59.3037" TopMargin="16.4746" BottomMargin="631.5254" LeftEage="297" RightEage="297" TopEage="51" BottomEage="51" Scale9OriginX="297" Scale9OriginY="51" Scale9Width="308" Scale9Height="53" ctype="ImageViewObjectData">
                <Size X="800.0000" Y="120.0000" />
                <Children>
                  <AbstractNodeData Name="Text_1" ActionTag="-352895011" Tag="90" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="327.5000" RightMargin="327.5000" TopMargin="39.0000" BottomMargin="39.0000" FontSize="36" LabelText="Question" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="145.0000" Y="42.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="400.0000" Y="60.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="26" G="26" B="26" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.1813" Y="0.3500" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="564.6963" Y="691.5254" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5515" Y="0.9004" />
                <PreSize X="0.7813" Y="0.1563" />
                <FileData Type="Normal" Path="games/ToMau/question.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="board" ActionTag="-2038146564" Tag="91" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="51.5000" RightMargin="51.5000" TopMargin="138.0000" BottomMargin="138.0000" LeftEage="274" RightEage="274" TopEage="135" BottomEage="135" Scale9OriginX="274" Scale9OriginY="135" Scale9Width="373" Scale9Height="222" ctype="ImageViewObjectData">
                <Size X="921.0000" Y="492.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="384.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5000" />
                <PreSize X="0.8994" Y="0.6406" />
                <FileData Type="Normal" Path="games/ToMauImg/mauDo_12.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="right_panel" ActionTag="-1193336645" Tag="139" IconVisible="False" LeftMargin="58.4911" RightMargin="165.5089" TopMargin="677.4063" BottomMargin="-89.4063" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="800.0000" Y="180.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="458.4911" Y="0.5937" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.4477" Y="0.0008" />
                <PreSize X="0.7813" Y="0.2344" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="Panel_2" ActionTag="1678840198" Tag="20" IconVisible="False" LeftMargin="244.3609" RightMargin="249.6391" TopMargin="161.5119" BottomMargin="168.4881" TouchEnable="True" ClipAble="False" BackColorAlpha="162" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="530.0000" Y="438.0000" />
                <Children>
                  <AbstractNodeData Name="Image_6_0" ActionTag="-1888682643" Alpha="5" CallBackType="Touch" CallBackName="onTouchObject" Tag="131" IconVisible="False" LeftMargin="417.2840" RightMargin="3.7160" TopMargin="139.5075" BottomMargin="156.4925" TouchEnable="True" LeftEage="12" RightEage="12" TopEage="20" BottomEage="20" Scale9OriginX="12" Scale9OriginY="20" Scale9Width="85" Scale9Height="102" ctype="ImageViewObjectData">
                    <Size X="109.0000" Y="142.0000" />
                    <Children>
                      <AbstractNodeData Name="red" ActionTag="633547521" VisibleForFrame="False" Tag="92" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="40.0000" RightMargin="40.0000" TopMargin="59.5000" BottomMargin="59.5000" FontSize="20" LabelText="red" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="29.0000" Y="23.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="54.5000" Y="71.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="0.2661" Y="0.1620" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint />
                    <Position X="417.2840" Y="156.4925" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.7873" Y="0.3573" />
                    <PreSize X="0.2057" Y="0.3242" />
                    <FileData Type="Normal" Path="games/ToMauImg/mauDo_10.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_3" ActionTag="1382762100" Alpha="5" CallBackType="Touch" CallBackName="onTouchObject" Tag="16" IconVisible="False" LeftMargin="413.9439" RightMargin="-10.9439" TopMargin="121.1679" BottomMargin="156.8321" TouchEnable="True" LeftEage="27" RightEage="27" TopEage="29" BottomEage="29" Scale9OriginX="27" Scale9OriginY="29" Scale9Width="73" Scale9Height="102" ctype="ImageViewObjectData">
                    <Size X="127.0000" Y="160.0000" />
                    <Children>
                      <AbstractNodeData Name="red" ActionTag="615314797" VisibleForFrame="False" Tag="91" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="49.0000" RightMargin="49.0000" TopMargin="68.5000" BottomMargin="68.5000" FontSize="20" LabelText="red" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="29.0000" Y="23.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="63.5000" Y="80.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="0.2283" Y="0.1437" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint />
                    <Position X="413.9439" Y="156.8321" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.7810" Y="0.3581" />
                    <PreSize X="0.2396" Y="0.3653" />
                    <FileData Type="Normal" Path="games/ToMauImg/mauDo_11.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_6" ActionTag="-356835940" Alpha="5" CallBackType="Touch" CallBackName="onTouchObject" Tag="19" IconVisible="False" LeftMargin="-41.8875" RightMargin="398.8875" TopMargin="105.4080" BottomMargin="138.5920" TouchEnable="True" LeftEage="12" RightEage="12" TopEage="20" BottomEage="20" Scale9OriginX="12" Scale9OriginY="20" Scale9Width="149" Scale9Height="154" ctype="ImageViewObjectData">
                    <Size X="173.0000" Y="194.0000" />
                    <Children>
                      <AbstractNodeData Name="red" ActionTag="974241753" VisibleForFrame="False" Tag="90" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="72.0000" RightMargin="72.0000" TopMargin="85.5000" BottomMargin="85.5000" FontSize="20" LabelText="red" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="29.0000" Y="23.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="86.5000" Y="97.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="0.1676" Y="0.1186" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint />
                    <Position X="-41.8875" Y="138.5920" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="-0.0790" Y="0.3164" />
                    <PreSize X="0.3264" Y="0.4429" />
                    <FileData Type="Normal" Path="games/ToMauImg/mauDo_1.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_2" ActionTag="-2120259861" Alpha="5" CallBackType="Touch" CallBackName="onTouchObject" Tag="15" IconVisible="False" LeftMargin="-114.0788" RightMargin="480.0788" TopMargin="147.5549" BottomMargin="122.4451" TouchEnable="True" LeftEage="15" RightEage="15" TopEage="5" BottomEage="5" Scale9OriginX="15" Scale9OriginY="5" Scale9Width="134" Scale9Height="158" ctype="ImageViewObjectData">
                    <Size X="164.0000" Y="168.0000" />
                    <Children>
                      <AbstractNodeData Name="red" ActionTag="832575326" VisibleForFrame="False" Tag="98" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="67.5000" RightMargin="67.5000" TopMargin="72.5000" BottomMargin="72.5000" FontSize="20" LabelText="red" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="29.0000" Y="23.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="82.0000" Y="84.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="0.1768" Y="0.1369" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint />
                    <Position X="-114.0788" Y="122.4451" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="-0.2152" Y="0.2796" />
                    <PreSize X="0.3094" Y="0.3836" />
                    <FileData Type="Normal" Path="games/ToMauImg/mauDo_2.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_4" CanEdit="False" ActionTag="-1143914025" CallBackType="Touch" CallBackName="onTouchObject" Tag="17" IconVisible="False" LeftMargin="-119.5446" RightMargin="388.5446" TopMargin="101.3201" BottomMargin="118.6799" LeftEage="7" RightEage="7" TopEage="8" BottomEage="8" Scale9OriginX="7" Scale9OriginY="8" Scale9Width="247" Scale9Height="202" ctype="ImageViewObjectData">
                    <Size X="261.0000" Y="218.0000" />
                    <AnchorPoint />
                    <Position X="-119.5446" Y="118.6799" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="-0.2256" Y="0.2710" />
                    <PreSize X="0.4925" Y="0.4977" />
                    <FileData Type="Normal" Path="games/ToMauImg/mauDo_13.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_6_2" ActionTag="-260640584" Alpha="5" CallBackType="Touch" CallBackName="onTouchObject" Tag="133" IconVisible="False" LeftMargin="191.7995" RightMargin="175.2005" TopMargin="233.6145" BottomMargin="151.3855" TouchEnable="True" LeftEage="12" RightEage="12" TopEage="20" BottomEage="20" Scale9OriginX="12" Scale9OriginY="20" Scale9Width="139" Scale9Height="13" ctype="ImageViewObjectData">
                    <Size X="163.0000" Y="53.0000" />
                    <Children>
                      <AbstractNodeData Name="red" ActionTag="882732320" VisibleForFrame="False" Tag="96" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="67.0000" RightMargin="67.0000" TopMargin="15.0000" BottomMargin="15.0000" FontSize="20" LabelText="red" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="29.0000" Y="23.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="81.5000" Y="26.5000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="0.1779" Y="0.4340" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint />
                    <Position X="191.7995" Y="151.3855" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.3619" Y="0.3456" />
                    <PreSize X="0.3075" Y="0.1210" />
                    <FileData Type="Normal" Path="games/ToMauImg/mauDo_4.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_1" ActionTag="778456474" Alpha="5" CallBackType="Touch" CallBackName="onTouchObject" Tag="14" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="214.1200" RightMargin="190.8800" TopMargin="120.6874" BottomMargin="193.3126" TouchEnable="True" LeftEage="55" RightEage="55" TopEage="54" BottomEage="54" Scale9OriginX="55" Scale9OriginY="54" Scale9Width="15" Scale9Height="16" ctype="ImageViewObjectData">
                    <Size X="125.0000" Y="124.0000" />
                    <Children>
                      <AbstractNodeData Name="red" ActionTag="925886100" VisibleForFrame="False" Tag="97" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="48.0000" RightMargin="48.0000" TopMargin="50.5000" BottomMargin="50.5000" FontSize="20" LabelText="red" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="29.0000" Y="23.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="62.5000" Y="62.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="0.2320" Y="0.1855" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint />
                    <Position X="214.1200" Y="193.3126" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.4040" Y="0.4414" />
                    <PreSize X="0.2358" Y="0.2831" />
                    <FileData Type="Normal" Path="games/ToMauImg/mauDo_6.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_6_2_2" ActionTag="-2003090983" Alpha="5" CallBackType="Touch" CallBackName="onTouchObject" Tag="136" IconVisible="False" LeftMargin="317.4456" RightMargin="167.5544" TopMargin="116.0345" BottomMargin="276.9655" TouchEnable="True" LeftEage="12" RightEage="12" TopEage="20" BottomEage="20" Scale9OriginX="12" Scale9OriginY="20" Scale9Width="21" Scale9Height="5" ctype="ImageViewObjectData">
                    <Size X="45.0000" Y="45.0000" />
                    <Children>
                      <AbstractNodeData Name="red" ActionTag="-900750513" VisibleForFrame="False" Tag="95" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="8.0000" RightMargin="8.0000" TopMargin="11.0000" BottomMargin="11.0000" FontSize="20" LabelText="red" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="29.0000" Y="23.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="22.5000" Y="22.5000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="0.6444" Y="0.5111" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint />
                    <Position X="317.4456" Y="276.9655" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5990" Y="0.6323" />
                    <PreSize X="0.0849" Y="0.1027" />
                    <FileData Type="Normal" Path="games/ToMauImg/mauDo_5.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_6_1" CanEdit="False" ActionTag="1456600848" CallBackType="Touch" CallBackName="onTouchObject" Tag="132" IconVisible="False" LeftMargin="189.1744" RightMargin="164.8256" TopMargin="113.3829" BottomMargin="149.6171" LeftEage="12" RightEage="12" TopEage="20" BottomEage="20" Scale9OriginX="12" Scale9OriginY="20" Scale9Width="152" Scale9Height="135" ctype="ImageViewObjectData">
                    <Size X="176.0000" Y="175.0000" />
                    <AnchorPoint />
                    <Position X="189.1744" Y="149.6171" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.3569" Y="0.3416" />
                    <PreSize X="0.3321" Y="0.3995" />
                    <FileData Type="Normal" Path="games/ToMauImg/mauDo_3.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_vien" ActionTag="2131951706" Alpha="5" CallBackType="Touch" CallBackName="onTouchObject" Tag="18" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="527.0760" RightMargin="-108.0760" TopMargin="94.3820" BottomMargin="191.6180" TouchEnable="True" LeftEage="22" RightEage="22" TopEage="36" BottomEage="36" Scale9OriginX="22" Scale9OriginY="36" Scale9Width="67" Scale9Height="80" ctype="ImageViewObjectData">
                    <Size X="111.0000" Y="152.0000" />
                    <Children>
                      <AbstractNodeData Name="red" ActionTag="-18761221" VisibleForFrame="False" Tag="94" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="41.0000" RightMargin="41.0000" TopMargin="64.5000" BottomMargin="64.5000" FontSize="20" LabelText="red" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="29.0000" Y="23.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="55.5000" Y="76.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="0.2613" Y="0.1513" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="582.5760" Y="267.6180" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="1.0992" Y="0.6110" />
                    <PreSize X="0.2094" Y="0.3470" />
                    <FileData Type="Normal" Path="games/ToMauImg/mauDo_9.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_6_2_1" ActionTag="-290673816" Alpha="5" CallBackType="Touch" CallBackName="onTouchObject" Tag="135" RotationSkewX="0.2273" RotationSkewY="0.2264" IconVisible="False" LeftMargin="534.5568" RightMargin="-107.5568" TopMargin="104.8803" BottomMargin="189.1197" TouchEnable="True" LeftEage="12" RightEage="12" TopEage="20" BottomEage="20" Scale9OriginX="12" Scale9OriginY="20" Scale9Width="79" Scale9Height="104" ctype="ImageViewObjectData">
                    <Size X="103.0000" Y="144.0000" />
                    <Children>
                      <AbstractNodeData Name="red" ActionTag="-2117630017" VisibleForFrame="False" Tag="93" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="37.0000" RightMargin="37.0000" TopMargin="60.5000" BottomMargin="60.5000" FontSize="20" LabelText="red" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="29.0000" Y="23.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="51.5000" Y="72.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="0.2816" Y="0.1597" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint />
                    <Position X="534.5568" Y="189.1197" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="1.0086" Y="0.4318" />
                    <PreSize X="0.1943" Y="0.3288" />
                    <FileData Type="Normal" Path="games/ToMauImg/mauDo_8.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_6_2_0" CanEdit="False" ActionTag="833943310" CallBackType="Touch" CallBackName="onTouchObject" Tag="134" IconVisible="False" LeftMargin="418.3250" RightMargin="-111.3250" TopMargin="100.6084" BottomMargin="154.3916" LeftEage="12" RightEage="12" TopEage="20" BottomEage="20" Scale9OriginX="12" Scale9OriginY="20" Scale9Width="199" Scale9Height="143" ctype="ImageViewObjectData">
                    <Size X="223.0000" Y="183.0000" />
                    <AnchorPoint />
                    <Position X="418.3250" Y="154.3916" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.7893" Y="0.3525" />
                    <PreSize X="0.4208" Y="0.4178" />
                    <FileData Type="Normal" Path="games/ToMauImg/mauDo_7.png" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint />
                <Position X="244.3609" Y="168.4881" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.2386" Y="0.2194" />
                <PreSize X="0.5176" Y="0.5703" />
                <SingleColor A="255" R="26" G="26" B="26" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.0000" Y="1.0000" />
            <SingleColor A="255" R="150" G="200" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>