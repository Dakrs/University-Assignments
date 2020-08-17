import { createSwitchNavigator, createAppContainer } from 'react-navigation'
import { createStackNavigator } from 'react-navigation-stack'

import Login from './Screens/Login/Login'
import Home from './Screens/Home/Home'
import AuthLoadingScreen from './Screens/AuthLoadingScreen'
import Register from './Screens/Register'

const StackNavigator = createStackNavigator(
  {
    Home,
  },
  {
    initialRouteName: 'Home',
    defaultNavigationOptions: {
      headerShown:false

    },
  },
);

const StackNavigatorContainer = createAppContainer(StackNavigator);

const AuthStack = createStackNavigator(
  {
    SignIn: Login,
    App: StackNavigatorContainer,
    SignUp: Register
  },
  {
    initialRouteName: 'SignIn',
    headerMode: 'none',
    headerShown: false,
  },
);

const RootStack = createSwitchNavigator(
  {
    AuthLoading: AuthLoadingScreen,
    Auth: AuthStack,
    App: StackNavigatorContainer,
  },
  {
    initialRouteName: 'AuthLoading',
  },
);

const RootStackContainer = createAppContainer(RootStack);

export default RootStackContainer;
