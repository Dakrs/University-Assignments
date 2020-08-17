
import React, {useState,useEffect } from 'react';
import NavigationBar from 'react-native-navbar';
import {View,StyleSheet} from 'react-native';
import { IconButton,Menu,Divider} from 'react-native-paper';




const titleConfig = {
    title: 'Your ToDos',
  };
 

  const NavBar = ({SortByOrigin,SortByNormal,SortByDate})=> {

    const[show, setShow] = useState(false)
  
    _openOptions = () => setShow(true);
  
    _closeOptions = () => setShow(false);
  
    return (
        <View styles={style.container}>
        <NavigationBar
        title={titleConfig}
        rightButton={
          <Menu
          visible={show}
          onDismiss={_closeOptions}
          anchor={
            <IconButton icon="dots-vertical" onPress={_openOptions}>Show menu</IconButton>
          }
        >
          <Menu.Item onPress={() => {SortByNormal()
          _closeOptions()}} title="Reset" />
          <Divider/>
          <Menu.Item onPress={() => {SortByDate()
          _closeOptions()}} title="Time" />
          <Divider/>
          <Menu.Item onPress={() => {SortByOrigin()
          _closeOptions()}} title="Origin" />

        </Menu>
          }/>
      </View>
    )
  }

export const style = StyleSheet.create({
  container: {
      flex: 1,
    },
})

export default NavBar;