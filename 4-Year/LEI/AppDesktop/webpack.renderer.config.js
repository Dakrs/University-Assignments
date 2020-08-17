const rules = require('./webpack.rules');
const HtmlWebpackPlugin = require("html-webpack-plugin");
const path = require('path');
const webpack = require('webpack');

//CSS
rules.push({
  test: /\.css$/,
  use: [
    { loader: 'style-loader' },
    { loader: 'css-loader' },
    {
      loader: 'postcss-loader',
      options: {
        plugins: function () {
            return [
                  require('autoprefixer')
            ];
        }
      }
    },
    { loader: 'sass-loader' }
  ],
});

//Fonts
rules.push({
  test: /\.(woff|woff2|eot|ttf|otf)$/,
  use: ['file-loader',],
});

//Images
rules.push({
  test: /\.(png|svg|jpg|gif)$/,
  use: ['file-loader',],
});

module.exports = {
  // Put your normal webpack config below here
  /**
  entry: {
    loading_screen: './src/LoadingScreen/loading.js'
  },*/
  resolve: {
    alias: {
      vue: 'vue/dist/vue.min.js' ,
    }
  },
  module: {
    rules,
  },
  /**
  plugins: [
    new HtmlWebpackPlugin({
            hash: true,
            title: 'My Awesome application',
            myPageHeader: 'Hello World',
            template: './src/LoadingScreen/loading.html',
            chunks: ['loading_screen'],
            filename: 'loading_screen/loading.html',
        }),
    new webpack.DefinePlugin({
      'LOADING_SCREEN_WEBPACK_ENTRY': 'loading_screen/loading.html',
    }),
  ]*/
};
