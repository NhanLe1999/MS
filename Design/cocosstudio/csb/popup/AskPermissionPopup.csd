<GameFile>
  <PropertyGroup Name="AskPermissionPopup" Type="Layer" ID="429f6a7a-4cc9-475a-877e-d8a495805c89" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Layer" CustomClassName="AskPermissionPopup" Tag="46" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="background" ActionTag="774992520" Tag="65" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="164.7360" RightMargin="59.2640" TopMargin="122.0928" BottomMargin="45.9072" LeftEage="15" RightEage="15" TopEage="15" BottomEage="15" Scale9OriginX="15" Scale9OriginY="15" Scale9Width="678" Scale9Height="568" ctype="ImageViewObjectData">
            <Size X="800.0000" Y="600.0000" />
            <Children>
              <AbstractNodeData Name="lb.onboard.infomation" ActionTag="1799714123" Tag="84" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="258.0000" RightMargin="258.0000" TopMargin="44.5000" BottomMargin="494.5000" FontSize="50" LabelText="Thông báo" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="284.0000" Y="61.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="400.0000" Y="525.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="51" G="153" B="204" />
                <PrePosition X="0.5000" Y="0.8750" />
                <PreSize X="0.3550" Y="0.1017" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Bold.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="label.ask.for.permission" ActionTag="344900086" Tag="85" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" LeftMargin="80.0000" RightMargin="80.0001" TopMargin="168.0000" BottomMargin="293.9999" IsCustomSize="True" FontSize="30" LabelText="Ba mẹ hãy cho phép ứng dụng truy cập Microphone để bé tập phát âm cùng Monkey Junior nhé!" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="639.9999" Y="138.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="1.0000" />
                <Position X="399.9999" Y="432.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="85" G="85" B="85" />
                <PrePosition X="0.5000" Y="0.7200" />
                <PreSize X="0.8000" Y="0.2300" />
                <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="img.mic" ActionTag="-266471292" Tag="86" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentWidthEnabled="True" LeftMargin="287.7207" RightMargin="285.1606" TopMargin="274.9600" BottomMargin="107.0400" LeftEage="66" RightEage="66" TopEage="71" BottomEage="71" Scale9OriginX="66" Scale9OriginY="71" Scale9Width="69" Scale9Height="76" ctype="ImageViewObjectData">
                <Size X="227.1186" Y="218.0000" />
                <AnchorPoint ScaleX="0.5000" />
                <Position X="401.2800" Y="107.0400" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5016" Y="0.1784" />
                <PreSize X="0.2839" Y="0.3633" />
                <FileData Type="Normal" Path="popup/popup_mic.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="Button" ActionTag="699884870" CallBackType="Click" CallBackName="onOkButtonClicked" Tag="136" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="280.0000" RightMargin="280.0000" TopMargin="521.0000" BottomMargin="17.0000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="25" RightEage="25" TopEage="20" BottomEage="20" Scale9OriginX="25" Scale9OriginY="20" Scale9Width="190" Scale9Height="22" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="240.0000" Y="62.0000" />
                <Children>
                  <AbstractNodeData Name="Ok" ActionTag="1115965543" Tag="137" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="90.0000" RightMargin="90.0000" TopMargin="5.5258" BottomMargin="13.4742" FontSize="35" LabelText="OK" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="60.0000" Y="43.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="120.0000" Y="34.9742" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5641" />
                    <PreSize X="0.2500" Y="0.6935" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Bold.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="255" B="255" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="400.0000" Y="48.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.0800" />
                <PreSize X="0.3000" Y="0.1033" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Normal" Path="account/new_v3/button_save.png" Plist="" />
                <NormalFileData Type="Normal" Path="account/new_v3/button_save.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="564.7360" Y="345.9072" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5515" Y="0.4504" />
            <PreSize X="0.7813" Y="0.7813" />
            <FileData Type="Normal" Path="popup/popupBg.png" Plist="" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>