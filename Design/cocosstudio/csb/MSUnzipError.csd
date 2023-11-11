<GameFile>
  <PropertyGroup Name="MSUnzipError" Type="Layer" ID="57aceb0b-5f40-4710-9dc7-5601d854c254" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="90" Speed="1.0000">
        <Timeline ActionTag="377993313" Property="Scale">
          <ScaleFrame FrameIndex="0" X="0.0100" Y="0.0100">
            <EasingData Type="26" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="30" X="1.1000" Y="1.1000">
            <EasingData Type="26" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="60" X="1.1000" Y="1.1000">
            <EasingData Type="25" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="90" X="0.0100" Y="0.0100">
            <EasingData Type="25" />
          </ScaleFrame>
        </Timeline>
      </Animation>
      <AnimationList>
        <AnimationInfo Name="show" StartIndex="0" EndIndex="35">
          <RenderColor A="255" R="255" G="240" B="245" />
        </AnimationInfo>
        <AnimationInfo Name="hide" StartIndex="60" EndIndex="95">
          <RenderColor A="255" R="106" G="90" B="205" />
        </AnimationInfo>
      </AnimationList>
      <ObjectData Name="layer" CustomClassName="UnzipErrorPopup" Tag="31" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="layout" ActionTag="-1628935586" Tag="167" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" ClipAble="False" BackColorAlpha="204" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <AnchorPoint />
            <Position />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="1.0000" Y="1.0000" />
            <SingleColor A="255" R="0" G="0" B="0" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="alert_layout" ActionTag="377993313" Tag="281" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="272.0000" RightMargin="272.0000" TopMargin="201.9300" BottomMargin="92.0700" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="480.0000" Y="474.0000" />
            <Children>
              <AbstractNodeData Name="alert_bg" ActionTag="1282880789" Tag="282" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" LeftMargin="-116.5440" RightMargin="-118.0800" TopMargin="-209.6028" BottomMargin="-90.5340" LeftEage="15" RightEage="15" TopEage="15" BottomEage="15" Scale9OriginX="15" Scale9OriginY="15" Scale9Width="570" Scale9Height="620" ctype="ImageViewObjectData">
                <Size X="714.6240" Y="774.1368" />
                <Children>
                  <AbstractNodeData Name="noti_text" ActionTag="-1928686772" Tag="284" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentHeightEnable="True" PercentHeightEnabled="True" LeftMargin="123.2434" RightMargin="111.3806" TopMargin="416.7953" BottomMargin="125.1005" IsCustomSize="True" FontSize="25" LabelText="Học liệu đã chọn sẽ mất đi khi ba mẹ thoát ra ngoài. zxcklzxjclkjzxklcj zxlckjzlxjkc zxcjkl cjlzxkj zlxkjczlxkcj zlkxjclzxjkc" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="480.0000" Y="232.2410" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="363.2434" Y="241.2210" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="26" G="26" B="26" />
                    <PrePosition X="0.5083" Y="0.3116" />
                    <PreSize X="0.6717" Y="0.3000" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Regular.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="240.7680" Y="296.5344" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5016" Y="0.6256" />
                <PreSize X="1.4888" Y="1.6332" />
                <FileData Type="Normal" Path="mjstory/popup/popup_bg_new.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="cancel_button" ActionTag="-516122329" CallBackType="Click" CallBackName="onClose" Tag="7" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="498.9000" RightMargin="-93.9000" TopMargin="24.5890" BottomMargin="374.4010" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="45" Scale9Height="53" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="75.0000" Y="75.0100" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="536.4000" Y="411.9060" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="1.1175" Y="0.8690" />
                <PreSize X="0.1563" Y="0.1582" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Normal" Path="mjstory/papgemenucontrol_btclose.png" Plist="" />
                <NormalFileData Type="Normal" Path="mjstory/papgemenucontrol_btclose.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="button_continue" ActionTag="554156548" CallBackType="Click" CallBackName="onContinue" Tag="287" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" LeftMargin="-70.8480" RightMargin="-73.2480" TopMargin="407.4741" BottomMargin="-40.6455" TouchEnable="True" FontSize="14" ButtonText="XXXXXX" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="494" Scale9Height="68" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="624.0960" Y="107.1714" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="241.2000" Y="12.9402" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5025" Y="0.0273" />
                <PreSize X="1.3002" Y="0.2261" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Normal" Path="mjstory/popup/right_button.png" Plist="" />
                <NormalFileData Type="Normal" Path="mjstory/popup/popup_btok.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="exclamationIcon" ActionTag="820510148" Tag="289" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" LeftMargin="176.2800" RightMargin="176.2800" TopMargin="72.4272" BottomMargin="289.6140" LeftEage="35" RightEage="35" TopEage="31" BottomEage="31" Scale9OriginX="35" Scale9OriginY="31" Scale9Width="37" Scale9Height="32" ctype="ImageViewObjectData">
                <Size X="127.4400" Y="111.9588" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="240.0000" Y="345.5934" />
                <Scale ScaleX="0.8604" ScaleY="0.8604" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.7291" />
                <PreSize X="0.2655" Y="0.2362" />
                <FileData Type="Normal" Path="mjstory/material/mat_icon_warning_big.png" Plist="" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="329.0700" />
            <Scale ScaleX="1.1808" ScaleY="1.1808" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.4285" />
            <PreSize X="0.4688" Y="0.6172" />
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