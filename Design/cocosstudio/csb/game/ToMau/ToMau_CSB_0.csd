<GameFile>
  <PropertyGroup Name="ToMau_CSB" Type="Layer" ID="d091caf4-b4eb-4a57-869e-1893f52f10ea" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Layer" CustomClassName="ToMau" Tag="79" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="bg_1" ActionTag="-795817293" Tag="81" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="-336.6688" RightMargin="-339.3312" ctype="SpriteObjectData">
            <Size X="1700.0000" Y="768.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="513.3312" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5013" Y="0.5000" />
            <PreSize X="1.6602" Y="1.0000" />
            <FileData Type="Normal" Path="games/ToMau/BG.png" Plist="" />
            <BlendFunc Src="1" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="root_layout" ActionTag="-356526763" Tag="80" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" TopMargin="-0.0001" BottomMargin="0.0001" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
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
              <AbstractNodeData Name="board" ActionTag="-2038146564" Tag="91" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="192.5000" RightMargin="192.5000" TopMargin="129.5000" BottomMargin="129.5000" LeftEage="274" RightEage="274" TopEage="135" BottomEage="135" Scale9OriginX="274" Scale9OriginY="135" Scale9Width="91" Scale9Height="239" ctype="ImageViewObjectData">
                <Size X="639.0000" Y="509.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="384.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5000" />
                <PreSize X="0.6240" Y="0.6628" />
                <FileData Type="Normal" Path="games/ToMauImg/CCN_11.png" Plist="" />
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
              <AbstractNodeData Name="Panel_2" ActionTag="1678840198" Tag="20" IconVisible="False" LeftMargin="244.3609" RightMargin="249.6392" TopMargin="161.5120" BottomMargin="168.4881" TouchEnable="True" ClipAble="False" BackColorAlpha="162" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="530.0000" Y="438.0000" />
                <Children>
                  <AbstractNodeData Name="Image_6" ActionTag="-356835940" CallBackType="Touch" CallBackName="onTouchObject" Tag="19" IconVisible="False" LeftMargin="151.0223" RightMargin="174.9777" TopMargin="22.2793" BottomMargin="60.7207" TouchEnable="True" LeftEage="12" RightEage="12" TopEage="20" BottomEage="20" Scale9OriginX="12" Scale9OriginY="20" Scale9Width="180" Scale9Height="315" ctype="ImageViewObjectData">
                    <Size X="204.0000" Y="355.0000" />
                    <AnchorPoint />
                    <Position X="151.0223" Y="60.7207" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.2849" Y="0.1386" />
                    <PreSize X="0.3849" Y="0.8105" />
                    <FileData Type="Normal" Path="games/ToMauImg/CCN_0.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_6_0" ActionTag="-1888682643" CallBackType="Touch" CallBackName="onTouchObject" Tag="131" IconVisible="False" LeftMargin="1.7942" RightMargin="355.2058" TopMargin="286.4336" BottomMargin="68.5664" TouchEnable="True" LeftEage="12" RightEage="12" TopEage="20" BottomEage="20" Scale9OriginX="12" Scale9OriginY="20" Scale9Width="149" Scale9Height="43" ctype="ImageViewObjectData">
                    <Size X="173.0000" Y="83.0000" />
                    <AnchorPoint />
                    <Position X="1.7942" Y="68.5664" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.0034" Y="0.1565" />
                    <PreSize X="0.3264" Y="0.1895" />
                    <FileData Type="Normal" Path="games/ToMauImg/CCN_4.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_3" ActionTag="1382762100" CallBackType="Touch" CallBackName="onTouchObject" Tag="16" IconVisible="False" LeftMargin="231.7776" RightMargin="216.2224" TopMargin="56.3934" BottomMargin="279.6066" TouchEnable="True" LeftEage="27" RightEage="27" TopEage="29" BottomEage="29" Scale9OriginX="27" Scale9OriginY="29" Scale9Width="28" Scale9Height="44" ctype="ImageViewObjectData">
                    <Size X="82.0000" Y="102.0000" />
                    <AnchorPoint />
                    <Position X="231.7776" Y="279.6066" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.4373" Y="0.6384" />
                    <PreSize X="0.1547" Y="0.2329" />
                    <FileData Type="Normal" Path="games/ToMauImg/CCN_1.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_4" ActionTag="-1143914025" CallBackType="Touch" CallBackName="onTouchObject" Tag="17" IconVisible="False" LeftMargin="292.9948" RightMargin="166.0052" TopMargin="61.2840" BottomMargin="303.7160" TouchEnable="True" LeftEage="7" RightEage="7" TopEage="8" BottomEage="8" Scale9OriginX="7" Scale9OriginY="8" Scale9Width="57" Scale9Height="57" ctype="ImageViewObjectData">
                    <Size X="71.0000" Y="73.0000" />
                    <AnchorPoint />
                    <Position X="292.9948" Y="303.7160" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5528" Y="0.6934" />
                    <PreSize X="0.1340" Y="0.1667" />
                    <FileData Type="Normal" Path="games/ToMauImg/CCN_2.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_2" ActionTag="-2120259861" CallBackType="Touch" CallBackName="onTouchObject" Tag="15" IconVisible="False" LeftMargin="298.9050" RightMargin="174.0950" TopMargin="113.7822" BottomMargin="307.2178" TouchEnable="True" LeftEage="15" RightEage="15" TopEage="5" BottomEage="5" Scale9OriginX="15" Scale9OriginY="5" Scale9Width="27" Scale9Height="7" ctype="ImageViewObjectData">
                    <Size X="57.0000" Y="17.0000" />
                    <AnchorPoint />
                    <Position X="298.9050" Y="307.2178" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5640" Y="0.7014" />
                    <PreSize X="0.1075" Y="0.0388" />
                    <FileData Type="Normal" Path="games/ToMauImg/CCN_3.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_6_1" ActionTag="1456600848" CallBackType="Touch" CallBackName="onTouchObject" Tag="132" IconVisible="False" LeftMargin="274.1039" RightMargin="147.8961" TopMargin="361.0202" BottomMargin="0.9798" TouchEnable="True" LeftEage="12" RightEage="12" TopEage="20" BottomEage="20" Scale9OriginX="12" Scale9OriginY="20" Scale9Width="84" Scale9Height="36" ctype="ImageViewObjectData">
                    <Size X="108.0000" Y="76.0000" />
                    <AnchorPoint />
                    <Position X="274.1039" Y="0.9798" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5172" Y="0.0022" />
                    <PreSize X="0.2038" Y="0.1735" />
                    <FileData Type="Normal" Path="games/ToMauImg/CCN_5.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_6_2" ActionTag="-260640584" CallBackType="Touch" CallBackName="onTouchObject" Tag="133" IconVisible="False" LeftMargin="154.2126" RightMargin="269.7874" TopMargin="367.7891" BottomMargin="7.2109" TouchEnable="True" LeftEage="12" RightEage="12" TopEage="20" BottomEage="20" Scale9OriginX="12" Scale9OriginY="20" Scale9Width="82" Scale9Height="23" ctype="ImageViewObjectData">
                    <Size X="106.0000" Y="63.0000" />
                    <AnchorPoint />
                    <Position X="154.2126" Y="7.2109" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.2910" Y="0.0165" />
                    <PreSize X="0.2000" Y="0.1438" />
                    <FileData Type="Normal" Path="games/ToMauImg/CCN_6.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_6_2_2" ActionTag="-2003090983" CallBackType="Touch" CallBackName="onTouchObject" Tag="136" IconVisible="False" LeftMargin="15.9226" RightMargin="349.0774" TopMargin="52.7790" BottomMargin="116.2210" TouchEnable="True" LeftEage="12" RightEage="12" TopEage="20" BottomEage="20" Scale9OriginX="12" Scale9OriginY="20" Scale9Width="141" Scale9Height="229" ctype="ImageViewObjectData">
                    <Size X="165.0000" Y="269.0000" />
                    <AnchorPoint />
                    <Position X="15.9226" Y="116.2210" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.0300" Y="0.2653" />
                    <PreSize X="0.3113" Y="0.6142" />
                    <FileData Type="Normal" Path="games/ToMauImg/CCN_7.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_1" ActionTag="778456474" CallBackType="Touch" CallBackName="onTouchObject" Tag="14" IconVisible="False" LeftMargin="91.5011" RightMargin="271.4989" TopMargin="133.8928" BottomMargin="139.1072" TouchEnable="True" LeftEage="55" RightEage="55" TopEage="54" BottomEage="54" Scale9OriginX="55" Scale9OriginY="54" Scale9Width="57" Scale9Height="57" ctype="ImageViewObjectData">
                    <Size X="167.0000" Y="165.0000" />
                    <AnchorPoint />
                    <Position X="91.5011" Y="139.1072" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.1726" Y="0.3176" />
                    <PreSize X="0.3151" Y="0.3767" />
                    <FileData Type="Normal" Path="games/ToMauImg/CCN_10.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_6_2_0" ActionTag="833943310" CallBackType="Touch" CallBackName="onTouchObject" Tag="134" IconVisible="False" LeftMargin="381.9228" RightMargin="1.0772" TopMargin="1.0587" BottomMargin="158.9413" TouchEnable="True" LeftEage="12" RightEage="12" TopEage="20" BottomEage="20" Scale9OriginX="12" Scale9OriginY="20" Scale9Width="123" Scale9Height="238" ctype="ImageViewObjectData">
                    <Size X="147.0000" Y="278.0000" />
                    <AnchorPoint />
                    <Position X="381.9228" Y="158.9413" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.7206" Y="0.3629" />
                    <PreSize X="0.2774" Y="0.6347" />
                    <FileData Type="Normal" Path="games/ToMauImg/CCN_8.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_6_2_1" ActionTag="-290673816" CallBackType="Touch" CallBackName="onTouchObject" Tag="135" IconVisible="False" LeftMargin="332.6760" RightMargin="75.3240" TopMargin="94.1567" BottomMargin="165.8433" TouchEnable="True" LeftEage="12" RightEage="12" TopEage="20" BottomEage="20" Scale9OriginX="12" Scale9OriginY="20" Scale9Width="98" Scale9Height="138" ctype="ImageViewObjectData">
                    <Size X="122.0000" Y="178.0000" />
                    <AnchorPoint />
                    <Position X="332.6760" Y="165.8433" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.6277" Y="0.3786" />
                    <PreSize X="0.2302" Y="0.4064" />
                    <FileData Type="Normal" Path="games/ToMauImg/CCN_9.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_vien" CanEdit="False" ActionTag="2131951706" Tag="18" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftEage="132" RightEage="132" TopEage="124" BottomEage="124" Scale9OriginX="132" Scale9OriginY="124" Scale9Width="266" Scale9Height="190" ctype="ImageViewObjectData">
                    <Size X="530.0000" Y="438.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="265.0000" Y="219.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="1.0000" Y="1.0000" />
                    <FileData Type="Normal" Path="games/ToMauImg/CCN_12.png" Plist="" />
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
            <Position X="512.0000" Y="384.0001" />
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