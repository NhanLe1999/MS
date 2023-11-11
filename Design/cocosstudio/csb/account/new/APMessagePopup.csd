<GameFile>
  <PropertyGroup Name="APMessagePopup" Type="Layer" ID="c83d40eb-83f7-426a-ac06-d72ce19e828c" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Layer" CustomClassName="APMessagePopup" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="popup_bg" ActionTag="-394346387" Tag="360" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="253.0000" RightMargin="253.0000" TopMargin="236.0000" BottomMargin="236.0000" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="15" BottomEage="15" Scale9OriginX="15" Scale9OriginY="15" Scale9Width="64" Scale9Height="52" ctype="ImageViewObjectData">
            <Size X="518.0000" Y="296.0000" />
            <Children>
              <AbstractNodeData Name="success" ActionTag="-1477669051" Tag="1251" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="206.5000" RightMargin="206.5000" TopMargin="63.0000" BottomMargin="128.0000" LeftEage="34" RightEage="34" TopEage="34" BottomEage="34" Scale9OriginX="34" Scale9OriginY="34" Scale9Width="37" Scale9Height="37" ctype="ImageViewObjectData">
                <Size X="105.0000" Y="105.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="259.0000" Y="180.5000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.6098" />
                <PreSize X="0.2027" Y="0.3547" />
                <FileData Type="Normal" Path="success.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="unsuccess" ActionTag="1667687935" VisibleForFrame="False" Tag="1253" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="206.0000" RightMargin="206.0000" TopMargin="63.0000" BottomMargin="128.0000" LeftEage="34" RightEage="34" TopEage="34" BottomEage="34" Scale9OriginX="34" Scale9OriginY="34" Scale9Width="38" Scale9Height="37" ctype="ImageViewObjectData">
                <Size X="106.0000" Y="105.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="259.0000" Y="180.5000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.6098" />
                <PreSize X="0.2046" Y="0.3547" />
                <FileData Type="Normal" Path="unsuccess.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="exit_button" ActionTag="-171288735" CallBackType="Click" CallBackName="onCancel" Tag="361" IconVisible="False" LeftMargin="488.6901" RightMargin="-17.6901" TopMargin="-17.3383" BottomMargin="265.3383" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="7" RightEage="7" TopEage="11" BottomEage="11" Scale9OriginX="7" Scale9OriginY="11" Scale9Width="33" Scale9Height="26" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="47.0000" Y="48.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.1901" Y="289.3383" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.9888" Y="0.9775" />
                <PreSize X="0.0907" Y="0.1622" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                <NormalFileData Type="Normal" Path="exit_2.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="title" ActionTag="1717790470" Tag="362" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="32.5000" RightMargin="32.5000" TopMargin="156.0000" BottomMargin="20.0000" IsCustomSize="True" FontSize="24" LabelText="Xác thực tài khoản không thành công" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="453.0000" Y="120.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="259.0000" Y="80.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="0" G="0" B="0" />
                <PrePosition X="0.5000" Y="0.2703" />
                <PreSize X="0.8745" Y="0.4054" />
                <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="0.5059" Y="0.3854" />
            <FileData Type="Normal" Path="account/white_bg.png" Plist="" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>