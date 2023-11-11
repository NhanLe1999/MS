<GameFile>
  <PropertyGroup Name="ChooseAvartar" Type="Layer" ID="5ce9b0d2-5de7-4720-a710-b41c8a5c0af4" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Layer" CustomClassName="ChooseAvatarScene" Tag="232" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="bg" ActionTag="-878515536" Tag="233" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="-388.0000" RightMargin="-388.0000" TouchEnable="True" ClipAble="False" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="1800.0000" Y="768.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.7578" Y="1.0000" />
            <SingleColor A="255" R="255" G="255" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="root_layout" ActionTag="2044317186" Tag="234" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" ClipAble="False" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <Children>
              <AbstractNodeData Name="lb.key.account.profile.avatar.title" ActionTag="-1821241782" Tag="256" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="261.0000" RightMargin="261.0000" TopMargin="48.0000" BottomMargin="676.0000" FontSize="36" LabelText="Chọn hình đại diện cho bé" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="502.0000" Y="44.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="698.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="1" G="196" B="248" />
                <PrePosition X="0.5000" Y="0.9089" />
                <PreSize X="0.4902" Y="0.0573" />
                <FontResource Type="Normal" Path="fonts/Montserrat-ExtraBold.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="listview" ActionTag="1667361433" Tag="259" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="-388.0000" RightMargin="-388.0000" TopMargin="170.2500" BottomMargin="170.2500" TouchEnable="True" ClipAble="True" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ScrollDirectionType="0" VerticalType="Align_VerticalCenter" ctype="ListViewObjectData">
                <Size X="1800.0000" Y="427.5000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="384.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5000" />
                <PreSize X="1.7578" Y="0.5566" />
                <SingleColor A="255" R="255" G="255" B="255" />
                <FirstColor A="255" R="150" G="150" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="continue-button" ActionTag="625762054" CallBackType="Click" CallBackName="onNext" Tag="257" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="227.0000" RightMargin="227.0000" TopMargin="644.2000" BottomMargin="29.8000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="51" RightEage="51" TopEage="27" BottomEage="31" Scale9OriginX="51" Scale9OriginY="27" Scale9Width="55" Scale9Height="36" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="570.0000" Y="94.0000" />
                <Children>
                  <AbstractNodeData Name="lb.free2paid.profileage.buttonnext" ActionTag="-1894128849" Tag="258" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentHeightEnable="True" PercentHeightEnabled="True" TopMargin="-7.5200" BottomMargin="13.1600" IsCustomSize="True" FontSize="32" LabelText="Tiếp tục" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="570.0000" Y="88.3600" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="285.0000" Y="57.3400" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.6100" />
                    <PreSize X="1.0000" Y="0.9400" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-ExtraBold.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="76.8000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.1000" />
                <PreSize X="0.5566" Y="0.1224" />
                <TextColor A="255" R="65" G="65" B="70" />
                <NormalFileData Type="Normal" Path="free2paid/buttons/button2.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.0000" Y="1.0000" />
            <SingleColor A="255" R="255" G="255" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="button_back" ActionTag="-1269257004" CallBackType="Click" CallBackName="returnPreviousScreen" Tag="254" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="20.4800" RightMargin="953.5200" TopMargin="15.3600" BottomMargin="697.6400" TouchEnable="True" FontSize="14" LeftEage="15" RightEage="15" TopEage="5" BottomEage="5" Scale9OriginX="-15" Scale9OriginY="-5" Scale9Width="30" Scale9Height="10" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
            <Size X="50.0000" Y="55.0000" />
            <Children>
              <AbstractNodeData Name="img_back" ActionTag="1653478267" Tag="255" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="-8.0000" RightMargin="-8.0000" TopMargin="1.3750" BottomMargin="-12.3750" LeftEage="21" RightEage="21" TopEage="21" BottomEage="21" Scale9OriginX="21" Scale9OriginY="21" Scale9Width="24" Scale9Height="24" ctype="ImageViewObjectData">
                <Size X="66.0000" Y="66.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="25.0000" Y="20.6250" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.3750" />
                <PreSize X="1.3200" Y="1.2000" />
                <FileData Type="Normal" Path="user_account/close_button.png" Plist="" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleY="1.0000" />
            <Position X="20.4800" Y="752.6400" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.0200" Y="0.9800" />
            <PreSize X="0.0488" Y="0.0716" />
            <TextColor A="255" R="65" G="65" B="70" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>