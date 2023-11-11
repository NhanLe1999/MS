<GameFile>
  <PropertyGroup Name="ToMau" Type="Layer" ID="5a874c46-f0d1-4269-8eb4-1ac0c740f37c" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Layer" CustomClassName="ToMau" Tag="79" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="bg_1" ActionTag="-795817293" Tag="81" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="-336.6688" RightMargin="-339.3312" TopMargin="1.3056" BottomMargin="-1.3056" ctype="SpriteObjectData">
            <Size X="1700.0000" Y="768.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="513.3312" Y="382.6944" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5013" Y="0.4983" />
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
              <AbstractNodeData Name="board" ActionTag="-2038146564" Tag="91" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="199.0000" RightMargin="199.0000" TopMargin="135.0000" BottomMargin="135.0000" LeftEage="15" RightEage="15" TopEage="15" BottomEage="15" Scale9OriginX="15" Scale9OriginY="15" Scale9Width="596" Scale9Height="468" ctype="ImageViewObjectData">
                <Size X="626.0000" Y="498.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="384.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5000" />
                <PreSize X="0.6113" Y="0.6484" />
                <FileData Type="Normal" Path="games/ToMauImg/BTH_VectorSmartObject3.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="right_panel" ActionTag="-1193336645" Tag="139" IconVisible="False" LeftMargin="413.7303" RightMargin="405.4697" TopMargin="680.7020" BottomMargin="-92.7020" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="204.8000" Y="180.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="516.1303" Y="-2.7020" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5040" Y="-0.0035" />
                <PreSize X="0.2000" Y="0.2344" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="Panel_2" ActionTag="1020729299" Tag="83" IconVisible="False" LeftMargin="376.2496" RightMargin="384.7504" TopMargin="180.5040" BottomMargin="164.4960" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ComboBoxIndex="1" ColorAngle="90.0000" ctype="PanelObjectData">
                <Size X="263.0000" Y="423.0000" />
                <Children>
                  <AbstractNodeData Name="Image_4" ActionTag="507726954" CallBackType="Touch" CallBackName="onTouchObject" Tag="84" IconVisible="False" LeftMargin="122.7159" RightMargin="7.2841" TopMargin="262.5117" BottomMargin="48.4883" TouchEnable="True" LeftEage="7" RightEage="7" TopEage="8" BottomEage="8" Scale9OriginX="7" Scale9OriginY="8" Scale9Width="119" Scale9Height="96" ctype="ImageViewObjectData">
                    <Size X="133.0000" Y="112.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="189.2159" Y="104.4883" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.7195" Y="0.2470" />
                    <PreSize X="0.5057" Y="0.2648" />
                    <FileData Type="Normal" Path="games/ToMauImg/BTH_Layer 0.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_1" ActionTag="366331193" CallBackType="Touch" CallBackName="onTouchObject" Tag="85" IconVisible="False" LeftMargin="37.8299" RightMargin="11.1701" TopMargin="0.2577" BottomMargin="221.7423" TouchEnable="True" LeftEage="70" RightEage="70" TopEage="66" BottomEage="66" Scale9OriginX="70" Scale9OriginY="66" Scale9Width="74" Scale9Height="69" ctype="ImageViewObjectData">
                    <Size X="214.0000" Y="201.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="144.8299" Y="322.2423" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5507" Y="0.7618" />
                    <PreSize X="0.8137" Y="0.4752" />
                    <FileData Type="Normal" Path="games/ToMauImg/BTH_5.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_3" ActionTag="178058485" CallBackType="Touch" CallBackName="onTouchObject" Tag="86" IconVisible="False" LeftMargin="75.7385" RightMargin="46.2615" TopMargin="34.6422" BottomMargin="245.3578" TouchEnable="True" LeftEage="46" RightEage="46" TopEage="29" BottomEage="29" Scale9OriginX="46" Scale9OriginY="29" Scale9Width="49" Scale9Height="85" ctype="ImageViewObjectData">
                    <Size X="141.0000" Y="143.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="146.2385" Y="316.8578" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5560" Y="0.7491" />
                    <PreSize X="0.5361" Y="0.3381" />
                    <FileData Type="Normal" Path="games/ToMauImg/BTH_4.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_6" ActionTag="1142034754" CallBackType="Touch" CallBackName="onTouchObject" Tag="87" IconVisible="False" LeftMargin="101.4940" RightMargin="75.5060" TopMargin="61.8718" BottomMargin="275.1282" TouchEnable="True" LeftEage="12" RightEage="12" TopEage="20" BottomEage="20" Scale9OriginX="12" Scale9OriginY="20" Scale9Width="62" Scale9Height="46" ctype="ImageViewObjectData">
                    <Size X="86.0000" Y="86.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="144.4940" Y="318.1282" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5494" Y="0.7521" />
                    <PreSize X="0.3270" Y="0.2033" />
                    <FileData Type="Normal" Path="games/ToMauImg/BTH_3.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_6_0" ActionTag="280561214" CallBackType="Touch" CallBackName="onTouchObject" Tag="90" IconVisible="False" LeftMargin="122.6231" RightMargin="98.3769" TopMargin="83.6456" BottomMargin="297.3544" TouchEnable="True" LeftEage="12" RightEage="12" TopEage="20" BottomEage="20" Scale9OriginX="12" Scale9OriginY="20" Scale9Width="18" Scale9Height="2" ctype="ImageViewObjectData">
                    <Size X="42.0000" Y="42.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="143.6231" Y="318.3544" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5461" Y="0.7526" />
                    <PreSize X="0.1597" Y="0.0993" />
                    <FileData Type="Normal" Path="games/ToMauImg/BTH_2.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_4_0" ActionTag="-1036724504" CallBackType="Touch" CallBackName="onTouchObject" Tag="91" IconVisible="False" LeftMargin="9.0091" RightMargin="151.9909" TopMargin="213.9492" BottomMargin="102.0508" TouchEnable="True" LeftEage="7" RightEage="7" TopEage="8" BottomEage="8" Scale9OriginX="7" Scale9OriginY="8" Scale9Width="88" Scale9Height="91" ctype="ImageViewObjectData">
                    <Size X="102.0000" Y="107.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="60.0091" Y="155.5508" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.2282" Y="0.3677" />
                    <PreSize X="0.3878" Y="0.2530" />
                    <FileData Type="Normal" Path="games/ToMauImg/BTH_1.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_vien" ActionTag="1718435484" Tag="89" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftEage="86" RightEage="86" TopEage="124" BottomEage="124" Scale9OriginX="86" Scale9OriginY="124" Scale9Width="91" Scale9Height="175" ctype="ImageViewObjectData">
                    <Size X="263.0000" Y="423.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="131.5000" Y="211.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="1.0000" Y="1.0000" />
                    <FileData Type="Normal" Path="games/ToMauImg/BTH_6.png" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint />
                <Position X="376.2496" Y="164.4960" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.3674" Y="0.2142" />
                <PreSize X="0.2568" Y="0.5508" />
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