<GameFile>
  <PropertyGroup Name="MJLicenseAlert" Type="Layer" ID="894db139-9762-4f6e-b1f3-2adbd4655e75" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Layer" CustomClassName="MJLicenseAlert" Tag="660" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="color_layout" ActionTag="-253160900" Alpha="0" Tag="81" IconVisible="False" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" RightMargin="-10.2400" TopMargin="-7.6800" TouchEnable="True" ClipAble="False" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="1034.2400" Y="775.6800" />
            <AnchorPoint />
            <Position />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="1.0100" Y="1.0100" />
            <SingleColor A="255" R="0" G="0" B="0" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="bg" ActionTag="267278555" Tag="261" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="212.0000" RightMargin="212.0000" TopMargin="193.9680" BottomMargin="154.0320" LeftEage="304" RightEage="304" TopEage="183" BottomEage="183" Scale9OriginX="269" Scale9OriginY="183" Scale9Width="35" Scale9Height="26" ctype="ImageViewObjectData">
            <Size X="600.0000" Y="420.0000" />
            <Children>
              <AbstractNodeData Name="Image_1" ActionTag="1822659550" Tag="100" IconVisible="False" LeftMargin="152.7444" RightMargin="163.2556" TopMargin="-88.7773" BottomMargin="350.7773" LeftEage="93" RightEage="93" TopEage="52" BottomEage="52" Scale9OriginX="93" Scale9OriginY="52" Scale9Width="98" Scale9Height="58" ctype="ImageViewObjectData">
                <Size X="284.0000" Y="158.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="294.7444" Y="429.7773" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.4912" Y="1.0233" />
                <PreSize X="0.4733" Y="0.3762" />
                <FileData Type="Normal" Path="license/logo.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="message" ActionTag="1400209164" Tag="103" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="50.0000" RightMargin="50.0000" TopMargin="79.5020" BottomMargin="110.4980" IsCustomSize="True" FontSize="29" LabelText="Không có kết nối mạng, vui lòng kiểm tra thiết bị và thử !" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="500.0000" Y="230.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="300.0000" Y="225.4980" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="54" G="127" B="180" />
                <PrePosition X="0.5000" Y="0.5369" />
                <PreSize X="0.8333" Y="0.5476" />
                <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="btcancel" ActionTag="45415735" CallBackType="Click" CallBackName="onCancelClicked" Tag="98" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="80.0000" RightMargin="320.0000" TopMargin="333.1478" BottomMargin="26.8522" TouchEnable="True" FontSize="25" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="186" Scale9Height="29" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="200.0000" Y="60.0000" />
                <Children>
                  <AbstractNodeData Name="lbcancel" ActionTag="815051586" Tag="99" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="46.5000" RightMargin="46.5000" TopMargin="11.5000" BottomMargin="11.5000" FontSize="30" LabelText="Cancel" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="107.0000" Y="37.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="100.0000" Y="30.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.5350" Y="0.6167" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="180.0000" Y="56.8522" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.3000" Y="0.1354" />
                <PreSize X="0.3333" Y="0.1429" />
                <TextColor A="255" R="255" G="255" B="255" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                <NormalFileData Type="Normal" Path="license/warningremoveview/warningremoveview_btok.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="btok" ActionTag="907000672" CallBackType="Click" CallBackName="onOkClicked" Tag="100" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="320.0000" RightMargin="80.0000" TopMargin="333.1478" BottomMargin="26.8522" TouchEnable="True" FontSize="25" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="186" Scale9Height="29" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="200.0000" Y="60.0000" />
                <Children>
                  <AbstractNodeData Name="lbok" ActionTag="1914132962" Tag="101" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="76.0000" RightMargin="76.0000" TopMargin="11.5000" BottomMargin="11.5000" FontSize="30" LabelText="Ok" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="48.0000" Y="37.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="100.0000" Y="30.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.2400" Y="0.6167" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="420.0000" Y="56.8522" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.7000" Y="0.1354" />
                <PreSize X="0.3333" Y="0.1429" />
                <TextColor A="255" R="255" G="255" B="255" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                <NormalFileData Type="Normal" Path="license/warningremoveview/warningremoveview_btclose.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="364.0320" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.4740" />
            <PreSize X="0.5859" Y="0.5469" />
            <FileData Type="Normal" Path="license/alert/licensealert_bg.png" Plist="" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>