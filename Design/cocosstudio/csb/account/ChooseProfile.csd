<GameFile>
  <PropertyGroup Name="ChooseProfile" Type="Layer" ID="1ea50e9d-3f4d-4f68-8efb-94b4e3fdb3b2" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Layer" CustomClassName="APChooseProfileView" Tag="13" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="Panel_1" ActionTag="522927257" Tag="14" IconVisible="False" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TopMargin="-1.4285" BottomMargin="1.4285" TouchEnable="True" ClipAble="True" ComboBoxIndex="1" ColorAngle="90.0000" LeftEage="548" RightEage="548" TopEage="253" BottomEage="253" Scale9OriginX="548" Scale9OriginY="253" Scale9Width="567" Scale9Height="262" ctype="PanelObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <Children>
              <AbstractNodeData Name="Text_1" ActionTag="-672200402" Tag="15" IconVisible="False" PositionPercentXEnabled="True" VerticalEdge="TopEdge" LeftMargin="164.0000" RightMargin="164.0000" TopMargin="77.5000" BottomMargin="623.5000" FontSize="60" LabelText="Choose your child's profile" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="696.0000" Y="67.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="657.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.8555" />
                <PreSize X="0.6797" Y="0.0872" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="button_lock" ActionTag="-872966733" CallBackType="Click" CallBackName="onLock" Tag="152" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="846.6000" RightMargin="27.4000" TopMargin="631.2000" BottomMargin="16.8000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="6" BottomEage="6" Scale9OriginX="-15" Scale9OriginY="-6" Scale9Width="30" Scale9Height="12" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="150.0000" Y="120.0000" />
                <Children>
                  <AbstractNodeData Name="image" ActionTag="-851419041" Tag="153" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="45.0000" RightMargin="45.0000" TopMargin="30.0000" BottomMargin="30.0000" LeftEage="19" RightEage="19" TopEage="19" BottomEage="19" Scale9OriginX="19" Scale9OriginY="19" Scale9Width="22" Scale9Height="22" ctype="ImageViewObjectData">
                    <Size X="60.0000" Y="60.0000" />
                    <Children>
                      <AbstractNodeData Name="sidebar.des.parents" ActionTag="-854377782" Tag="154" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="-75.0000" RightMargin="60.0000" TopMargin="36.0000" FontSize="20" LabelText="Parent" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="75.0000" Y="24.0000" />
                        <AnchorPoint ScaleX="1.0000" />
                        <Position />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition />
                        <PreSize X="1.2500" Y="0.4000" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-Bold.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="75.0000" Y="60.0000" />
                    <Scale ScaleX="1.2500" ScaleY="1.2500" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.4000" Y="0.5000" />
                    <FileData Type="Normal" Path="mjstory/lock.png" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="921.6000" Y="76.8000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.9000" Y="0.1000" />
                <PreSize X="0.1465" Y="0.1563" />
                <TextColor A="255" R="65" G="65" B="70" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="lvProfile" ActionTag="-1164838577" Tag="143" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="62.0000" RightMargin="62.0000" TopMargin="234.0000" BottomMargin="234.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ScrollDirectionType="0" ItemMargin="33" ctype="ListViewObjectData">
                <Size X="900.0000" Y="300.0000" />
                <Children>
                  <AbstractNodeData Name="Panel_2" ActionTag="-1297742456" Tag="21" IconVisible="False" RightMargin="700.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" LeftEage="41" RightEage="41" TopEage="40" BottomEage="40" Scale9OriginX="-41" Scale9OriginY="-40" Scale9Width="82" Scale9Height="80" ctype="PanelObjectData">
                    <Size X="200.0000" Y="300.0000" />
                    <Children>
                      <AbstractNodeData Name="Image_1" ActionTag="-1945521250" Tag="137" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="-25.0000" RightMargin="-25.0000" TopMargin="-25.0000" BottomMargin="-25.0000" Scale9Enable="True" LeftEage="60" RightEage="60" TopEage="60" BottomEage="60" Scale9OriginX="60" Scale9OriginY="60" Scale9Width="5" Scale9Height="4" ctype="ImageViewObjectData">
                        <Size X="250.0000" Y="350.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="100.0000" Y="150.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="1.2500" Y="1.1667" />
                        <FileData Type="Normal" Path="mjstory/new_ui/login/ap_profile_background.png" Plist="" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="Image_1" ActionTag="242583111" Tag="20" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" BottomMargin="100.0000" LeftEage="72" RightEage="72" TopEage="72" BottomEage="72" Scale9OriginX="72" Scale9OriginY="72" Scale9Width="76" Scale9Height="76" ctype="ImageViewObjectData">
                        <Size X="200.0000" Y="200.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="1.0000" />
                        <Position X="100.0000" Y="300.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="1.0000" />
                        <PreSize X="1.0000" Y="0.6667" />
                        <FileData Type="Normal" Path="mjstory/avatar/boy_2.png" Plist="" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="Text_2" ActionTag="821527795" Tag="29" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="72.5000" RightMargin="72.5000" TopMargin="200.0000" BottomMargin="60.0000" FontSize="35" LabelText="Jon" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="55.0000" Y="40.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="100.0000" Y="80.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="0" G="0" B="0" />
                        <PrePosition X="0.5000" Y="0.2667" />
                        <PreSize X="0.2750" Y="0.1333" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="Text_2_0" ActionTag="1211471222" Alpha="127" Tag="30" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="67.0000" RightMargin="67.0000" TopMargin="246.0000" BottomMargin="26.0000" FontSize="25" LabelText="Age 8" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="66.0000" Y="28.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="100.0000" Y="40.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="0" G="0" B="0" />
                        <PrePosition X="0.5000" Y="0.1333" />
                        <PreSize X="0.3300" Y="0.0933" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleY="0.5000" />
                    <Position Y="150.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition Y="0.5000" />
                    <PreSize X="0.2222" Y="1.0000" />
                    <SingleColor A="255" R="150" G="200" B="255" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Panel_2_0" ActionTag="-545133187" ZOrder="1" Tag="144" IconVisible="False" LeftMargin="233.0000" RightMargin="467.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" LeftEage="41" RightEage="41" TopEage="40" BottomEage="40" Scale9OriginX="-41" Scale9OriginY="-40" Scale9Width="82" Scale9Height="80" ctype="PanelObjectData">
                    <Size X="200.0000" Y="300.0000" />
                    <Children>
                      <AbstractNodeData Name="Image_1" ActionTag="-1284483822" Tag="145" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="-25.0000" RightMargin="-25.0000" TopMargin="-25.0000" BottomMargin="-25.0000" Scale9Enable="True" LeftEage="60" RightEage="60" TopEage="60" BottomEage="60" Scale9OriginX="60" Scale9OriginY="60" Scale9Width="5" Scale9Height="4" ctype="ImageViewObjectData">
                        <Size X="250.0000" Y="350.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="100.0000" Y="150.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="1.2500" Y="1.1667" />
                        <FileData Type="Normal" Path="mjstory/new_ui/login/ap_profile_background.png" Plist="" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="Image_1" ActionTag="-1534951790" Tag="146" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" BottomMargin="100.0000" LeftEage="72" RightEage="72" TopEage="72" BottomEage="72" Scale9OriginX="72" Scale9OriginY="72" Scale9Width="76" Scale9Height="76" ctype="ImageViewObjectData">
                        <Size X="200.0000" Y="200.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="1.0000" />
                        <Position X="100.0000" Y="300.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="1.0000" />
                        <PreSize X="1.0000" Y="0.6667" />
                        <FileData Type="Normal" Path="mjstory/avatar/boy_2.png" Plist="" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="Text_2" ActionTag="-174053676" Tag="147" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="72.5000" RightMargin="72.5000" TopMargin="200.0000" BottomMargin="60.0000" FontSize="35" LabelText="Jon" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="55.0000" Y="40.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="100.0000" Y="80.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="0" G="0" B="0" />
                        <PrePosition X="0.5000" Y="0.2667" />
                        <PreSize X="0.2750" Y="0.1333" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="Text_2_0" ActionTag="-1958537470" Alpha="127" Tag="148" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="67.0000" RightMargin="67.0000" TopMargin="246.0000" BottomMargin="26.0000" FontSize="25" LabelText="Age 8" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="66.0000" Y="28.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="100.0000" Y="40.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="0" G="0" B="0" />
                        <PrePosition X="0.5000" Y="0.1333" />
                        <PreSize X="0.3300" Y="0.0933" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleY="0.5000" />
                    <Position X="233.0000" Y="150.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.2589" Y="0.5000" />
                    <PreSize X="0.2222" Y="1.0000" />
                    <SingleColor A="255" R="150" G="200" B="255" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Panel_2_1" ActionTag="663698238" ZOrder="2" Tag="149" IconVisible="False" LeftMargin="466.0000" RightMargin="234.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" LeftEage="41" RightEage="41" TopEage="40" BottomEage="40" Scale9OriginX="-41" Scale9OriginY="-40" Scale9Width="82" Scale9Height="80" ctype="PanelObjectData">
                    <Size X="200.0000" Y="300.0000" />
                    <Children>
                      <AbstractNodeData Name="Image_1" ActionTag="-1181827874" Tag="150" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="-25.0000" RightMargin="-25.0000" TopMargin="-25.0000" BottomMargin="-25.0000" Scale9Enable="True" LeftEage="60" RightEage="60" TopEage="60" BottomEage="60" Scale9OriginX="60" Scale9OriginY="60" Scale9Width="5" Scale9Height="4" ctype="ImageViewObjectData">
                        <Size X="250.0000" Y="350.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="100.0000" Y="150.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="1.2500" Y="1.1667" />
                        <FileData Type="Normal" Path="mjstory/new_ui/login/ap_profile_background.png" Plist="" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="Image_1" ActionTag="1930287856" Tag="151" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" BottomMargin="100.0000" LeftEage="72" RightEage="72" TopEage="72" BottomEage="72" Scale9OriginX="72" Scale9OriginY="72" Scale9Width="76" Scale9Height="76" ctype="ImageViewObjectData">
                        <Size X="200.0000" Y="200.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="1.0000" />
                        <Position X="100.0000" Y="300.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="1.0000" />
                        <PreSize X="1.0000" Y="0.6667" />
                        <FileData Type="Normal" Path="mjstory/avatar/boy_2.png" Plist="" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="Text_2" ActionTag="-346523323" Tag="152" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="72.5000" RightMargin="72.5000" TopMargin="200.0000" BottomMargin="60.0000" FontSize="35" LabelText="Jon" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="55.0000" Y="40.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="100.0000" Y="80.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="0" G="0" B="0" />
                        <PrePosition X="0.5000" Y="0.2667" />
                        <PreSize X="0.2750" Y="0.1333" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="Text_2_0" ActionTag="-571560813" Alpha="127" Tag="153" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="67.0000" RightMargin="67.0000" TopMargin="246.0000" BottomMargin="26.0000" FontSize="25" LabelText="Age 8" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="66.0000" Y="28.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="100.0000" Y="40.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="0" G="0" B="0" />
                        <PrePosition X="0.5000" Y="0.1333" />
                        <PreSize X="0.3300" Y="0.0933" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleY="0.5000" />
                    <Position X="466.0000" Y="150.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5178" Y="0.5000" />
                    <PreSize X="0.2222" Y="1.0000" />
                    <SingleColor A="255" R="150" G="200" B="255" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Panel_2_2" ActionTag="-93401319" ZOrder="3" Tag="154" IconVisible="False" LeftMargin="699.0000" RightMargin="1.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" LeftEage="41" RightEage="41" TopEage="40" BottomEage="40" Scale9OriginX="-41" Scale9OriginY="-40" Scale9Width="82" Scale9Height="80" ctype="PanelObjectData">
                    <Size X="200.0000" Y="300.0000" />
                    <Children>
                      <AbstractNodeData Name="Image_1" ActionTag="-660239213" Tag="155" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="-25.0000" RightMargin="-25.0000" TopMargin="-25.0000" BottomMargin="-25.0000" Scale9Enable="True" LeftEage="60" RightEage="60" TopEage="60" BottomEage="60" Scale9OriginX="60" Scale9OriginY="60" Scale9Width="5" Scale9Height="4" ctype="ImageViewObjectData">
                        <Size X="250.0000" Y="350.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="100.0000" Y="150.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="1.2500" Y="1.1667" />
                        <FileData Type="Normal" Path="mjstory/new_ui/login/ap_profile_background.png" Plist="" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="Image_1" ActionTag="-709522229" Tag="156" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" BottomMargin="100.0000" LeftEage="72" RightEage="72" TopEage="72" BottomEage="72" Scale9OriginX="72" Scale9OriginY="72" Scale9Width="76" Scale9Height="76" ctype="ImageViewObjectData">
                        <Size X="200.0000" Y="200.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="1.0000" />
                        <Position X="100.0000" Y="300.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="1.0000" />
                        <PreSize X="1.0000" Y="0.6667" />
                        <FileData Type="Normal" Path="mjstory/avatar/boy_2.png" Plist="" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="Text_2" ActionTag="-1804441585" Tag="157" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="72.5000" RightMargin="72.5000" TopMargin="200.0000" BottomMargin="60.0000" FontSize="35" LabelText="Jon" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="55.0000" Y="40.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="100.0000" Y="80.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="0" G="0" B="0" />
                        <PrePosition X="0.5000" Y="0.2667" />
                        <PreSize X="0.2750" Y="0.1333" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="Text_2_0" ActionTag="1884195504" Alpha="127" Tag="158" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="67.0000" RightMargin="67.0000" TopMargin="246.0000" BottomMargin="26.0000" FontSize="25" LabelText="Age 8" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="66.0000" Y="28.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="100.0000" Y="40.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="0" G="0" B="0" />
                        <PrePosition X="0.5000" Y="0.1333" />
                        <PreSize X="0.3300" Y="0.0933" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleY="0.5000" />
                    <Position X="699.0000" Y="150.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.7767" Y="0.5000" />
                    <PreSize X="0.2222" Y="1.0000" />
                    <SingleColor A="255" R="150" G="200" B="255" />
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
                <PreSize X="0.8789" Y="0.3906" />
                <SingleColor A="255" R="150" G="150" B="255" />
                <FirstColor A="255" R="150" G="150" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="Panel_3" ActionTag="1765958296" Tag="195" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="12.0000" RightMargin="12.0000" TopMargin="602.8000" BottomMargin="65.2000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="1000.0000" Y="100.0000" />
                <Children>
                  <AbstractNodeData Name="btn_create" ActionTag="2019557596" CallBackType="Click" CallBackName="onNewProfile" Tag="45" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="318.5000" RightMargin="318.5000" TopMargin="5.0000" BottomMargin="5.0000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="22" RightEage="22" TopEage="22" BottomEage="22" Scale9OriginX="22" Scale9OriginY="22" Scale9Width="1" Scale9Height="1" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="363.0000" Y="90.0000" />
                    <Children>
                      <AbstractNodeData Name="Text_1_0_0" ActionTag="1778549887" Tag="46" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="19.0000" RightMargin="19.0000" TopMargin="25.5000" BottomMargin="25.5000" FontSize="32" LabelText="Create New Profiles" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="325.0000" Y="39.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="181.5000" Y="45.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="30" G="144" B="255" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="0.8953" Y="0.4333" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="500.0000" Y="50.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.3630" Y="0.9000" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                    <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                    <NormalFileData Type="Normal" Path="mjstory/new_ui/button_intro_grade_white.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="115.2000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.1500" />
                <PreSize X="0.9766" Y="0.1302" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint />
            <Position Y="1.4285" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition Y="0.0019" />
            <PreSize X="1.0000" Y="1.0000" />
            <FileData Type="Normal" Path="bg.jpg" Plist="" />
            <SingleColor A="255" R="255" G="255" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="Panel_4" ActionTag="980873979" CallBackType="Click" CallBackName="onClose" Tag="170" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" RightMargin="924.0000" BottomMargin="728.0000" TouchEnable="True" ClipAble="False" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="100.0000" Y="40.0000" />
            <Children>
              <AbstractNodeData Name="Text_1_0_0" ActionTag="-1933499341" Tag="171" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="28.0000" RightMargin="28.0000" TopMargin="8.5000" BottomMargin="8.5000" FontSize="20" LabelText="Back" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="44.0000" Y="23.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="50.0000" Y="20.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5000" />
                <PreSize X="0.4400" Y="0.5750" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleY="1.0000" />
            <Position Y="768.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition Y="1.0000" />
            <PreSize X="0.0977" Y="0.0521" />
            <SingleColor A="255" R="30" G="144" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="Image_2" ActionTag="-233910765" VisibleForFrame="False" Tag="276" IconVisible="False" PositionPercentYEnabled="True" LeftMargin="117.5000" RightMargin="891.5000" TopMargin="366.5000" BottomMargin="366.5000" FlipX="True" Scale9Enable="True" LeftEage="1" RightEage="1" TopEage="1" BottomEage="1" Scale9OriginX="1" Scale9OriginY="1" Scale9Width="57" Scale9Height="123" ctype="ImageViewObjectData">
            <Size X="15.0000" Y="35.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="125.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.1221" Y="0.5000" />
            <PreSize X="0.0146" Y="0.0456" />
            <FileData Type="Normal" Path="mjstory/new_ui/login/icon_back.png" Plist="" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>